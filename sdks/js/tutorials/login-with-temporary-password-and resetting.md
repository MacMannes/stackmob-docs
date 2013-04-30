<h3>Objectives</h3>
Login with given temporary password from the email sent by StackMob and set a new password

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object with `Bill Watterson` as its `username` (create using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create tutorial</a>)

* User `Bill Watterson` has received a <a href="https://developer.stackmob.com/tutorials/js/Forgot-Password" target="_blank">temporary password</a> from StackMob

* You have access to Bill Watterson's email address

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-loginwithtempandsetnewpassword" target="_blank">loginWithTempAndSetNewPassword</a>

<h2>Login With Temporary Password And Set a New Password</h2>

Bill Watterson has forgotten his password and we need to help him to recover it.

He clicked on our "Forgot Password" link already and has received an email containing a temporary password from StackMob.

He wants to login with that temporary password and change his password at the same time.

```js
var user = new StackMob.User({ username: 'Bill Watterson' });
user.loginWithTempAndSetNewPassword('temporary_password_from_StackMob_email', 'new_password', false, {
	success: function(model) {
		// now password for user 'Bill Watterson' has changed
		console.debug(model);
	},
	error: function(model, response) {
		console.debug(response['error']);
	}
});
```