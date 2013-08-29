Want to just see it in action? Check out the [PushDemo example app](https://github.com/stackmob/stackmob-android-examples)

<h2>Objective</h2>

Register an app for push with StackMob, and send/receive a message

<h2>Experience Level</h2>
Beginner

<h2>Estimated time to complete</h2>
~15 minutes

<h2>Prerequisites</h2>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

* A [Google Cloud Messaging Account](https://developer.stackmob.com/tutorials/android/Push-Notifications)

<h1>Let's get started!</h1>

<h2>Introduction to Android Push Notifications</h2>

StackMob integrates with Google's GCM service for push notifications on Android. Before you start using push notifications for Android on StackMob, you should familiarize yourself with the client-side aspects of GCM by reading the [Google Cloud Messaging Framework](http://developer.android.com/guide/google/gcm/gs.html) guide. Don't worry about the server-side aspects of push - that's what we're here for.

<h2>Android Push Notifications on StackMob</h2>

StackMob helps you with your Push Notification process by providing you the backend to send push messages. Follow these steps to get Push running for your Android application.

1. Create a Google API project, enable GCM, and generate your API Key following the directions at [http://developer.android.com/guide/google/gcm/gs.html](http://developer.android.com/guide/google/gcm/gs.html)
1. Note the "sender id" and "API Key" you generate. These will be used later
1. [Upload](https://dashboard.stackmob.com/module/push/settings/android) your API Key to StackMob.
1. Get a valid Android registration ID for your device for testing.
1. Send push notifications to your Android device from StackMob's web console.

<h2>Generating and Uploading your API Key</h2>

To use the push service you'll need to obtain a API Key from Google and [upload it to the StackMob server](https://dashboard.stackmob.com/module/push/settings/android). Upload your API Key to Sandbox. When you deploy your app to Production, you'll want to upload an API Key there as well. It can be the same one or a different one.

<h2>Sending and Receiving Push Messages</h2>

Now that you have an API Key, let's test sending & receiving push notifications. First, make sure yourn API Key is correctly saved [here](https://dashboard.stackmob.com/module/push/settings/android). Next, set up an Android app. You'll do the appropriate setup and write the code to create & register a registration ID and to receive push messages.


<h2>App Setup</h2>

If you're using the <a href="https://github.com/stackmob/stackmob-android-examples">Android Starter Project</a>, GCM Push is already set up for you. Just set up your [StackMob credentials](https://developer.stackmob.com/stackmob-android-sdk/configure), and add your sender id to the top of <a href="https://github.com/stackmob/stackmob-android-examples/blob/master/PushDemo/src/com/stackmob/pushdemo/PushDemoActivity.java" target="_blank">`PushDemoActivity.java`</a>

```java
	public static String SENDER_ID = "YOUR_SENDER_ID_HERE";
```

and you're ready to go. You can grow your app for the starter, or rip it apart for the bits you want

You can also set up your app manually, using [Google's instructions](http://developer.android.com/guide/google/gcm/gs.html). The StackMob sdk already contains gcm.jar

Note, that StackMob Push isn't set up for OAuth2 yet, so you'll have to add the OAuth1 initialization.

```java
	StackMobAndroid.init(this.getApplicationContext(), StackMob.OAuthVersion.One, 0, "YOUR_API_KEY_HERE", "YOUR_API_SECRET_HERE");
```

The demo push project is already set up for this, so you won't need to make any changes there.

<h3>Interacting with StackMob</h3>

In the `onRegistered` callback of `GCMIntentService` you get the registration id, which is the last piece you need to start talking to stackmob. Check out the [Push Docs](http://stackmob.com/devcenter/docs/Push-API) for examples of how to use StackMob to register your device and start pushing.

<h3>PushTokens</h3>
`StackMobPushToken` represents the information needed to identify a device for push. You can construct them with the "registration id" you get back from registering your device with GCM. By default it assume GCM, but you can also work with C2DM or iOS tokens if you need to.

<h3>Registering for push</h3>
StackMob keeps a collection of push tokens for your app, and you must add a new token to the collection before you can send messages to that device. You can associate the token with a user, which allows you to think in terms of users rather than tokens. A user can have multiple tokens on different devices, and the tokens can be expired and replaced by GCM at any time on the server, so this is good practice. All tokens on the server will be used when you do a broadcast message.

```java
User user = new User("<username>", "<optional password>");
user.registerForPush(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```

<h3>Unregister for push</h3>
When a user unsubscribes from push messages you'll want to remove their tokens from StackMob

```java
user.removeFromPush(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```

<h3>Get a user's tokens</h3>
If you do want to see the raw tokens for a user you can do so:

```java
user.getPushTokens(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```
The callback will be invoked with a json array that can be serialized to `StackMobPushToken`


<h3>Pushing to a user</h3>
To actually send a push to a specific user, create a map with your information, and make this call.
```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
user.sendPush(payload, standardToastCallback);
```

You can send a message to multiple users at once also

```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
User.pushToMultiple(payload, Arrays.asList(user1, user2, user3), standardToastCallback);
```

It's simple to save your `User` objects, query for them to get various groupings, and then send push messages.

<h3>Broadcast push message</h3>

Sending a push message to all registered tokens for your app is called a broadcast

```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
StackMobPush.getPush().broadcastPushNotification(payload, standardToastCallback);
```

<h3>Push without users</h3>

If you just want to do push without worrying about users (if you're only ever doing broadcasts for example) you can find a lower level API in [StackMobPush](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html). You can user an arbitrary string for username if you don't plan on using it at all

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobUser](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html) and [StackMobPush](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)

<h3>Push in Development vs. Production</h3>

Once you set your version to 1 and use the production keys, StackMob will use Apple's production push server 
instead of the Apple sandbox.

You should only use the dev keys with version 0 and the production keys with version 1.

The user tokens don't change, so you would still test in the same manner.

Here is some additional info on <a href="http://developer.stackmob.com/tutorials/dashboard/API-Versioning">API versioning with development and production environments</a>.
