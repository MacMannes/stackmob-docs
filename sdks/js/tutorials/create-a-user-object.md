<h3>Objective</h3>
Create a new `user` object.

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-create" target="_blank">create</a>

<h2>Create a User Object</h2>

```js
var user = new StackMob.User({ username: 'Bill Watterson', password: 'weirdosfromanotherplanet', profession: 'cartoonist'  });
user.create();
```

<h2>Callback</h2>
Remember that we can add callbacks to our API call!
Take our example above:

```js
user.create({
	success: function(model) {
		console.debug('User object is saved, username: ' + model.get('username'));
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```