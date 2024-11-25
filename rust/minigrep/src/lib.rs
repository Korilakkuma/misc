use std::env;
use std::error::Error;
use std::fs::File;
use std::io::prelude::*;

pub struct Config {
    pub query: String,
    pub filename: String,
    pub case_sensitive: bool,
}

impl Config {
    pub fn new(mut args: std::env::Args) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("cargo run 'query' 'filename'");
        }

        args.next();

        let query = match args.next() {
            Some(arg) => arg,
            None => return Err("cargo run 'query' 'filename'"),
        };

        let filename = match args.next() {
            Some(arg) => arg,
            None => return Err("cargo run 'query' 'filename'"),
        };

        let case_sensitive = env::var("CASE_INSENSITIVE").is_err();

        Ok(Config { query, filename, case_sensitive })
    }
}

pub fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let mut file = File::open(config.filename)?;

    let mut contents = String::new();

    file.read_to_string(&mut contents)?;

    let found_lines = if config.case_sensitive {
        search(&config.query, &contents)
    } else {
        search_case_insensitive(&config.query, &contents)
    };

    for line in found_lines {
        println!("{}", line);
    }

    Ok(())
}

pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    contents.lines()
        .filter(|line| {
            line.contains(query)
        })
        .collect()
}

pub fn search_case_insensitive<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    let lower_query = query.to_lowercase();

    let mut vec = Vec::new();

    for line in contents.lines() {
        if line.to_lowercase().contains(&lower_query) {
            vec.push(line);
        }
    }

    vec

    /*
    contents.lines()
        .map(|line| {
            line.to_lowercase()
        })
        .filter(|line| {
            line.contains(&lower_query)
        })
        .collect()
    */
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn one_result() {
        let query = "duct";

        let contents ="\
Rust:
safe, fast, productive.
Pick three.";

        assert_eq!(vec!["safe, fast, productive."], search(query, contents));
    }

    #[test]
    fn case_sensitive() {
        let query = "duct";

        let contents ="\
Rust:
safe, fast, productive.
Pick three.
Duct tape.";

        assert_eq!(vec!["safe, fast, productive."], search(query, contents));
    }

    #[test]
    fn case_insensitive() {
        let query = "rUsT";

        let contents = "\
Rust:
safe, fast, productive.
Pick three.
Trust me.";

        assert_eq!(vec!["Rust:", "Trust me."], search_case_insensitive(query, contents));
    }
}
