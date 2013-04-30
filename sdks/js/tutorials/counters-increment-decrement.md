<h3>Objectives</h3>
Use increment and decrement on save on one of the `todo` objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `Todo` object which `todo_id` is `todo1` (create using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-an-Object" target="_blank">create tutorial</a>)

* `Todo` object has field `num_likes` as an integer

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-incrementonsave" target="_blank">incrementOnSave</a>

* <a href="https://developer.stackmob.com/sdks/js/api#a-decrementonsave" target="_blank">decrementOnSave</a>

* <a href="https://developer.stackmob.com/sdks/js/api#a-save" target="_blank">save</a>

<h2>Increment num_likes Field</h2>

Let's assume we have a social `Todo` list app where people have an option to like and unlike any `Todo` list.

Hence, we want to keep track how many likes we have gotten so far by incrementing our `num_likes` field as user clicks on like button.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1' });
myTodo.incrementOnSave('num_likes', 1); // the field 'num_likes' will be incremented by 1
myTodo.save({}, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
	// response['error'] is the container for our error message
		console.debug("Aww...why did you fail on me?! " + response['error']);
	}
});
```

<h2>Decrement num_likes Field</h2>

Now, we want to deal with the scenario of user clicking the unlike button. Hence, we are going to use decrement on save.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1' });
myTodo.decrementOnSave('num_likes', 1); // the field 'num_likes' will be decremented by 1
myTodo.save({}, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
	// response['error'] is the container for our error message
		console.debug("Aww...why did you fail on me?! " + response['error']);
	}
});
```

Note: these increment and decrement on save methods are useful in some game apps, say you want to update a character's health point when it takes damage or uses health potion.