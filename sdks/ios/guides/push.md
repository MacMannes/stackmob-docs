iOS Push Notifications Guide
================

## Creating your Push Certificate

Before you can send push notifications, you need to create a Push Certificate for your app and upload it to StackMob.
<br>

Login to the <a href="http://developer.apple.com" target="_blank">Apple Developer Site</a> and click on the iOS DevCenter

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-06.png">
<br><br>

Click on the iOS Provisioning Portal

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-07.png">
<br><br>
We are going to assume you have your Developer Certificate and Device configured for development.  If you haven't, follow the directions in the Provisioning Portal.
<br/><br/>

Click on **App IDs** on the left menu, then click **New App ID** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-08.png">
<br><br>
Give your app a common name (which can be anything).  Next, enter the unique identifier for you app, 
This needs to match the name when you created the app in XCode.  In our example, it's **com.stackmob.push-notification**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-09.png">
<br><br>
Now that you've created you app, let's configure it for push.  Click the **configure link**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-10.png">
<br><br>
Check the box for **Enable for Apple Push Notification Service** and click the **Configure** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-11.png">
<br><br>
You'll be prompted to generate a **Certificate Signing Request (CSR)** from your **Keychain Access** Application.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-12.png">

<br><br>
Launch the **Keychain Access application** found in Applications > Utilities. From the **Keychain Access** menu, select **Certificate Assistant > Request a Certificate from a Certificate Authority**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-14.png">
<br><br>
Save the certificate to disk.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-15.png">
<br><br>
Go back to the **Apple Developer site** and click the continue button, if you haven't already.
Click **choose file** and select the certificate you just created and click **Generate**

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-16.png">
<br><br>
Click **continue** and **Download the aps_development.cer file** to your computer.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-17.png">
<br><br>
**Double-click the aps_development.cer** file and it will be added to your Keychain.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-18.png">

## Export Push Certificate

Next, you'll export the Push Certificate (.p12) file for upload to StackMob.

In the Keychain Access application, 
**right-click** on your Push Service certificate and select **Export Apple Development iOS Push Services:...**

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-19.png">
<br><br>
Give the file a unique name and save it as a .p12 file.  You'll be prompted to give it a password.  Make sure you remember the password, you'll need it when you upload the file to StackMob.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-20.png">
<br><br>
Open a browser to the StackMob Dashboard and navigate to <a href="https://dashboard.stackmob.com/module/push/settings/ios" target="_blank">Manage Push Credentials</a>.  
Click on **Upload New Certs** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-23.png">
<br><br>
Choose the .p12 file you created and **enter the certificate password**.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-24.png">


## Create Provisioning Profile

Click the **Provisioning** link on the left side and click the **New Profile** button.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-25.png">
<br><br>
Enter a **Profile Name**, Check the box for your developer certificate, select your newly created **App ID** and the device you will use for testing.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-26.png">
<br><br>
Click the **Download** button and save the Provisioning file on your computer.


<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-27.png">
<br><br>
**Open the XCode Organizer** (located under the Window menu in XCode) and drag and drop the **.mobileprovision** file onto the Provisioning Profiles area.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-28.png">
<br><br>
Return to your app in XCode and under **Build Settings**, search for "signing".  Under **Code Signing Identity** debug, you'll select the mobile provisioning with the bundle name that matches your app.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/push-notification/push-29.png">

