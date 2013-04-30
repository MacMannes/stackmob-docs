<h3>Objectives</h3>
Read all `todo` objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-fetch" target="_blank">fetch</a>

<h2>Read All Todo Objects</h2>

Assuming you have created a few `Todo` objects, let's fetch all of them!

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo();
myTodo.fetch({
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```