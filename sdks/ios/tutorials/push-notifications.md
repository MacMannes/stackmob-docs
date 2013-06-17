Introduction to Apple Push Notifications
=======================================

Before you start using push notifications on StackMob, you should familiarize yourself with the client-side aspects of Apple Push Notification Service (APNS) by reading the <a target="_blank" href="https://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Introduction/Introduction.html#//apple_ref/doc/uid/TP40008194-CH1-SW1">Local Notifications and Push Notification Programming Guide</a>. Don't worry about the server-side aspects of push - that's what we're here for.

## Apple Push Notifications on StackMob

StackMob helps you with your Push Notification process by providing you the backend to send push messages. Follow these steps to get Push running for your mobile application.

1. Generate a .p12 Push Certificate for your app from Apple
2. Upload your Push Certificate to StackMob
3. Get a valid Device Token for your mobile device for testing
4. Send push notifications to your mobile device from StackMob's web console

## Getting a Push Certificate

To use the push service you'll need to obtain push certificates from Apple and upload them to the StackMob server.

Follow the instructions in <a target="_blank" href="https://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/ProvisioningDevelopment/ProvisioningDevelopment.html#//apple_ref/doc/uid/TP40008194-CH104-SW1">Apple's Push Notification Programming Guide</a> to create a certificate file in .p12 format. Follow the instructions to step 3 of "Installing the SSL Certificate and Key on the Server." When you export the certificate and key, you will be prompted for an optional password. **DO** provide a password. We will not be able to use a certificate without a password. **Skip steps 4 and 5.** StackMob servers require a .p12 file, not a .pem file. A development certificate will be enough to get started.

StackMob has a development and production environment for you. When you're interacting with the development version of StackMob's API, development certificates will be used to send push notifications. **When you're ready to deploy your app, you'll need to get a production certificate by following the same instructions.**

## Upload your Push Certificate to StackMob

Because StackMob will be handling your push services, you will need to <a href="https://dashboard.stackmob.com/module/push/settings/android" target="_blank">upload your push certificates to StackMob</a>.

<a href="https://dashboard.stackmob.com/module/push/settings/android" class="screenshot" target="_blank"><img src="https://dashboard.stackmob.com/images/tutorials/StackMob_Upload_Push_Certs.png"/></a>

Browse to your saved .p12 file, select the appropriate environment, enter the certificate password, and upload your certificates. If everything went fine, you'll see a page confirming that you have a Development certificate on file.

## Using iOS Push SDK Version 1.0.1

With the release of the new iOS SDK, all push support has been moved into a seperate SDK.

For documentation and downloads, visit the [SMPushClient class reference](http://stackmob.github.com/stackmob-ios-push-sdk/Classes/SMPushClient.html)

**You do not need to read ahead unless you are using v0.5.x of the iOS SDK!**

## Push support for iOS SDK Version 0.5.x

### Getting a valid Device Token for your mobile device

Now that you have a certificate, we want to test sending/receiving push notifications. To do so, you'll need to get a valid device token from Apple. You can retrieve one through a local test iOS app.

Set up an iOS app to register for remote notifications by calling `registerForRemoteNotificationTypes` when your app launches. Implement the method `application:didRegisterForRemoteNotificationsWithDeviceToken:` which will be called when your app successfully registers with APNS.

```objective-c
[[UIApplication sharedApplication] registerForRemoteNotificationTypes: 
     (UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
```

```objective-c
- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken 
{
    NSString *token = [[deviceToken description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    token = [[token componentsSeparatedByString:@" "] componentsJoinedByString:@""];
    // Persist your user's accessToken here if you need
    [[StackMob stackmob] registerForPushWithUser:currentUser.userName token:token andCallback:^(BOOL success, id result){
        if(success){
            // token saved sucessfully for user
        }
        else{
            // Unable to register device for PUSH notifications 
            // Failed.  Alert your delgates
        }
    }];
}
```

### Sending Push Messages

You can send push messages to any registered device token, or any user that has a device token associated with the account (using the `registerForPushWithUser:andToken:andCallback` method above). You can also broadcast a message to every registered device token.

To send a message to devices by token:

```objc
NSMutableDictionary *args = [NSMutableDictionary dictionaryWithCapacity:3];
[args setValue:@"my push message" forKey:@"alert"];
[args setValue:[NSNumber numberWithInt:1] forKey:@"badge"];

NSArray *tokens = // create NSArray of tokens you want to send a notification to

[[StackMob stackob] sendPushToTokensWithArguments:args withTokens:tokens andCallback:^(BOOL success, id result) {
  // success will be true if the notification was successfully queued
}]
```

To send a message to users:

```objc
NSMutableDictionary *args = [NSMutableDictionary dictionaryWithCapacity:3];
[args setValue:@"my push message" forKey:@"alert"];
[args setValue:[NSNumber numberWithInt:1] forKey:@"badge"];

NSArray *userIds = [NSArray arrayWithObjects:@"john", "mike", nil];

[[StackMob stackob] sendPushToUsersWithArguments:args withUserIds:userIds andCallback:^(BOOL success, id result) {
  // success will be true if the notification was successfully queued
}]
```

To send a broadcast to all registered devices: 

```objc
NSMutableDictionary *args = [NSMutableDictionary dictionaryWithCapacity:3];
[args setValue:@"my push message" forKey:@"alert"];
[args setValue:[NSNumber numberWithInt:1] forKey:@"badge"];

[[Stackmob stackmob] sendPushBroadcastWithArguments:args andCallback:^(BOOL success, id result) {
  // success will be true if the notification was successfully queued
}]
```
