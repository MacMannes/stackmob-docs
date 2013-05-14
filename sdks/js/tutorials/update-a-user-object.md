<h3>Objective</h3>
Update a `user` object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object with `username: 'Bill Watterson'` (create with <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create tutorial</a>)

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-save" target="_blank">save</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-set" target="_blank">set</a>

<h2>Update a User Object</h2>

Here is the simplest way to update a `User` object.

```js
// if there is a field that is not there yet, StackMob will automatically create it for you
var user = new StackMob.User({ username: 'Bill Watterson', profession: 'professional clown', age: 20 });
user.save({}, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

We can also put the fields that we want to update inside `save()`.

```js
var user = new StackMob.User({ username: 'Bill Watterson' });
user.save({ profession: 'consultant', age: 30}, {
	success: function(model) {
		console.log(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

Another way is to set the fields locally using `set()`.

```js
var user = new StackMob.User({ username: 'Bill Watterson' });
user.set({ profession: 'gogo dancer', age: 21 });
user.save({}, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```