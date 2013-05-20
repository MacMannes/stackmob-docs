# Analytics

## Objective
Adding a Binary field to schemas

## Experience Level
Beginner

## Estimated Time to Complete<
~5 minutes

## Prerequisites

* You have a StackMob account. If you don't, <a href="https://stackmob.com/signup" target="_blank">sign up</a> now!

* You have a StackMob application

* You have a schema. If you don't have any, then you need to <a href="https://dashboard.stackmob.com/schemas/new" target="_blank">create a schema</a>

* You have an Amazon Web Services account. If you don't, <a href="http://aws.amazon.com" target="_blank">sign up</a> now!  You'll store Binary data on Amazon S3.


# Let's get started!

## Setup an Amazon S3 Bucket

* <a href="http://aws.amazon.com" target="_blank">Login to the Amazon Web Services</a> portal
* Click on S3
* Click on create bucket button
* Give you bucket a name and click create


## Add S3 settings to your StackMob application
You'll need your AWS security credentials to allow StackMob to save data on S3.  


From the AWS account menu and select Security Credentials.  

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-01.png"></img>


Click on the link Access Credentials.  You'll be taken to your Access Keys.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-02.png"></img>



Copy the **Access Key Id** and **Secret Access Key** and paste them into the <a href="https://dashboard.stackmob.com/module/s3/settings">S3 Settings page</a> in the StackMob dashboard.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-03.png"></img>


Type in the name of the S3 bucket you just created.  You can leave the S3 Path Alias blank.  Click the **submit button**.


## Add a Binary Field

Go to <a href="https://dashboard.stackmob.com/schemas/">Manage Schemas</a> and click `Edit` link on one of the schemas that you would like to add a Binary  field.

Click Add Field button and in this example, we will name our Binary field as `photo`.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/s3-04.png"></img>


Click Add Field, then Save Schema, and you're set!