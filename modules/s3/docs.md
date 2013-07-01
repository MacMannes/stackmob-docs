Binary Data via Amazon S3
=========================

## Overview 



<h2>Setup an Amazon S3 Bucket</h2>

* <a href="http://aws.amazon.com" target="_blank">Login to the Amazon Web Services</a> portal
* Click on S3
* Click on create bucket button
* Give you bucket a name and click create


<h2>Add S3 settings to your StackMob application</h2>
You'll need your AWS security credentials to allow StackMob to save data on S3.  


From the AWS account menu and select Security Credentials.  

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-01.png"></img>

Once in the Security Credentials area, you'll need to create a new user.  You should download the security credentials which include your Access Key ID and Secret.  They will only be available upon user creation.

Next, you will grant the user Permission to read and write to your Amazon S3 bucket.


This video walks through creating a user and granting them full access to Amazon S3.

<p>
<iframe src="http://player.vimeo.com/video/69504620" width="500" height="281" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
</p>


Copy the **Access Key Id** and **Secret Access Key** and paste them into the <a href="https://dashboard.stackmob.com/module/s3/settings">S3 Settings page</a> in the StackMob dashboard.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-03.png"></img>


Type in the name of the S3 bucket you just created.  You can leave the S3 Path Alias blank.  Click the **submit button**.


<h2>Add a Binary Field</h2>

Go to <a href="https://dashboard.stackmob.com/schemas/">Manage Schemas</a> and click `Edit` link on one of the schemas that you would like to add a Binary  field.

Click Add Field button and in this example, we will name our Binary field as `photo`.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-04.png"></img>


Click Add Field, then Save Schema, and you're set!

## Using SDKs to Handle Files

Find more information on how StackMob's mobile SDKs handle files.

* <a href="https://developer.stackmob.com/ios-sdk/developer-guide#FileStorage" target="_blank">iOS SDK</a>
* <a href="https://developer.stackmob.com/android-sdk/developer-guide#FileStorage" target="_blank">Android SDK</a>
* <a href="https://developer.stackmob.com/js-sdk/developer-guide#FileStorage" target="_blank">JS SDK</a>

