'use strict';

import { initializeApp } from 'firebase/app';
import { getMessaging } from 'firebase/messaging';

const getFirebaseMessagingObject = () => {
  // Initialize Firebase
  const config = {
    // Maybe, `projectId` is required ...
    /* ... */
  };

  const firebase = initializeApp(config);

  return getMessaging(firebase);
};

const register = (messaging) => {
  if (!navigator.serviceWorker || !messaging) {
    return;
  }

  navigator.serviceWorker
    .register('./firebase-messaging-sw.js')
    .then(() => {
      return navigator.serviceWorker.ready;
    })
    .catch((error) => {
      console.error(error);
    })
    .then((registration) => {
      messaging.useServiceWorker(registration);

      messaging.requestPermission()
        .then(() => {
          console.log('Notification permission granted.');

          messaging.getToken()
            .then((token) => {
              console.log(token);

              const options = {
                  method : 'POST',
                  headers: new Headers({ 'Content-Type': 'application/json' }),
                  body   : JSON.stringify({ token })
              };

              fetch('/api/webpush/register', options)
                .then((res) => {
                  console.dir(res);
                }).catch((error) => {
                    console.error(error);
                });
            }).catch((error) => {
              console.error(error);
            });
        })
        .catch((error) => {
          console.log('Unable to get permission to notify.', error);
        });
    });
};

register(getFirebaseMessagingObject());
