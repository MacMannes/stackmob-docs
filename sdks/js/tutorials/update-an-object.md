<h3>Objective</h3>
Update a `todo` object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `Todo` object which `todo_id` is `todo1` (either using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-an-Object" target="_blank">create tutorial</a>)

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-save" target="_blank">save</a>

* <a href="https://developer.stackmob.com/sdks/js/api#a-set" target="_blank">set</a>

<h2>Update a Todo Object</h2>

We are going to update `title` field of a `Todo` object which id is `todo1`. We will change its `title` to become `try StackMob`.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1', title: 'try StackMob' });
myTodo.save({}, {
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
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1' });
myTodo.save({ title: 'try StackMob' }, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

Another way is to set the fields locally using `set()`.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ todo_id: 'todo1' });
myTodo.set({ title: 'try StackMob' });
myTodo.save({}, {
	success: function(model) {
		console.debug(model.toJSON());
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```