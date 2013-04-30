<h3>Objectives</h3>
Use forgot password

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object with `Bill Watterson` as its `username` and some valid email address in its `email` field (the field name can be anything as long as you correctly point the Forgot Password Email Field to the field containing email address)

* You have set the Forgot Password Email Field in your <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">`User` schema</a> to `email` (can be whichever as long as it contains email address)

* You have access to that email address

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-forgotpassword" target="_blank">forgotPassword</a>

<h2>Send an Email for Password Recovery</h2>

Bill Watterson has forgotten his password and we need to help him to recover it. We provide "Forgot Password" link on our webapp, and Bill Watterson clicks on it. Hence, we need to send him an email with instructions on how to recover his password.

```js
var user = new StackMob.User({ username: 'Bill Watterson' }); // assume Bill Watterson's email address is 'bwatterson@someemail.com'
user.forgotPassword({ // this will tell StackMob to send an email to Bill Watterson at 'bwatterson@someemail.com'
	success: function(model) {
		// redirect to a new page where 'Bill Watterson' can reset his password
		console.debug(model);
	},
	error: function(model, response) {
		console.debug(response['error']);
	}
});
```
