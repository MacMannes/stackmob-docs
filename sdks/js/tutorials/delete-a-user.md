<h3>Objectives</h3>
Delete a user object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>
* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-destroy" target="_blank">destroy</a>

<h2>Delete a User Object</h2>

Let's try to delete a `User` object which name is `Bill Watterson`

```js
var user = new StackMob.User({ username: 'Bill Watterson' });
user.destroy();
```

<h2>Callback</h2>
Remember that we can add callbacks to our API call!
Take our example above:

```js
user.destroy({
	success: function(model) {
		console.debug('User object is destroyed, username: ' + model.get('username'));
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```