<h3>Objectives</h3>
Reset password

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object with `Bill Watterson` as its `username` (create using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create tutorial</a>)

* User `Bill Watterson` is <a href="https://developer.stackmob.com/tutorials/js/User-Authentication" target="_blank">logged in</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-resetpassword" target="_blank">resetPassword</a>

<h2>Reset Password</h2>

Bill Watterson wants to change his password. He is logged in to StackMob.

```js
//"Bill Watterson" is already logged in
var user = new StackMob.User({ username: 'Bill Watterson' });
user.resetPassword('old_password', 'new_password', false, {
	success: function(model) {
		// now password for user 'Bill Watterson' has changed
		console.debug(model);
	},
	error: function(model, response) {
		console.debug(response['error']);
	}
});
```