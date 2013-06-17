Forgot Password
====================================================

## Overview 

StackMob gives you the ability to send Forgot Password emails.  These emails contain temporary passwords that are emailed to your users.  With these temporary passwords, the users can log into their account and reset their password.

The email is sourced from a field you have in the object instance.

<p class="screenshot">
  <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/forgotpassword/forgotpassword-link.png" alt="Select your email field in the User Schema"/></a>
</p>

You can also customize your emails with the <a href="https://dashboard.stackmob.com/module/forgotpassword/settings" target="_blank">Forgot Password Email Template</a> if you have the <a href="https://marketplace.stackmob.com/module/sendgrid" target="_blank">SendGrid Email Module</a> installed.  Without the SendGrid email module, forgot password emails are sent from a `@stackmob.com` email address.

The Forgot Password email template follows the <a href="http://scalate.fusesource.org/documentation/mustache.html" target="_blank">Mustache templating syntax</a>.

## Calling Forgot Password from the SDKs

Read about calling Forgot Password from the mobile SDKs:

* <a href="https://developer.stackmob.com/ios-sdk/developer-guide#PasswordRecovery" target="_blank">iOS SDK</a>
* <a href="https://developer.stackmob.com/android-sdk/developer-guide#ForgotPassword" target="_blank">Android SDK</a>
* <a href="https://developer.stackmob.com/js-sdk/developer-guide#ForgotPassword" target="_blank">JS SDK</a>
