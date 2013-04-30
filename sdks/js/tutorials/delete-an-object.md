<h3>Objectives</h3>
Delete an object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-destroy" target="_blank">destroy</a>

<h2>Delete an Object</h2>

Let's try to delete a `Todo` object which id is `todo1`.
Remember that we can only delete an object with its primary key (id).

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1'});
myTodo.destroy({
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```