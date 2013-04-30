Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Reset a user's password via email.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://dashboard.stackmob.com/sdks/android/config">set up for StackMob</a>

<h1>Let's get started!</h1>

Many websites and apps with a login process have a way to reset a user's password via email. StackMob provides a way to get this functionality into your app with minimal fuss. To do this you need to specify the field that represents an email field to which StackMob will send a temporary password. This can be done on your Edit Schema page for your `user` schema.

*From the <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">Edit User Schema page</a>:*
<p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/screenshots/overview/StackMob_Forgot_Password_User_Schema.png" alt="Forgot Password"/></p>

The email will be sent to the value of the field specified in your Forgot Password dropdown above.  If you selected `username` and this user's username was `example_john@stackmob.com`, then we'll send a temporary password to `example_john@stackmob.com`.

To kick off the process, have a button on your login screen labled "I forgot my password". That should lead to a screen prompting for a username. When that's submitted you can make this call

```java
StackMobUser.sendForgotPasswordEmail("johndoe", new StackMobModelCallback() {
	@Override public void success() {
	}

	@Override public void failure(StackMobException e) {
	}
});
```



The user will receive an email with a temporary password valid for 24 hours. The temporary password must be reset by the user before the user can login again. To do this after invoking the `sendForgotPasswordEmail` call, your app should redirect to a reset password screen with fields for the old and new password. The user can get the temporary password from their email, come back to your app, and submit the form. Once the form is submitted you can do a special login like below 

```java
StackMobUser user = new StackMobUser("johndoe", "tempPassword");
user.loginResettingTemporaryPassword("newPassword",  new StackMobModelCallback() {
	@Override public void success() {
		//login succeeded
	}
	@Override public void failure(StackMobException e) {
		//login failed
	}
});
```

Your user may also close the app and attempt to use the regular login screen with their temporary password. To handle this case your regular login call should override the special `temporaryPasswordResetRequired` method for its callback;

```java
final User user = new User("AzureDiamond", "hunter2");
user.login(new StackMobModelCallback() {

	@Override 
	public void temporaryPasswordResetRequired(StackMobException e) {
		//Prompt the user for a new password
		user.loginResettingTemporaryPassword("newPassword",  new StackMobModelCallback() {
			@Override public void success() {
				//login succeeded
			}
			@Override public void failure(StackMobException e) {
				//login failed
			}
		});
	}

	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
}
```

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobUser](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)