‘“<h3>Objective</h3>
Create a new object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-create" target="_blank">create</a>

<h2>Create a Todo Object</h2>

If you want to create an object other than `User`, we need to modify the code a little bit.

Say we want to create a `Todo` (this is our schema name) object.

Note: StackMob will create the schema for you if you haven't created it yet.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var myTodo = new Todo({ title: 'Send Data to StackMob!' });
myTodo.create();
```

<h2>Add More Fields</h2>
If you want to add more fields to your object, all you need to do is add more map keys.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
// completed will be a boolean field if it's not created already
var myTodo = new Todo({ title: 'Send Data to StackMob!', priority: 1, completed: true });
myTodo.create();
```

<h2>Callback</h2>
Remember that we can add callbacks to our API call!
Take our example above:

```js
myTodo.create({
	success: function(model) {
		console.debug('Todo object is saved, todo_id: ' + model.get('todo_id') + ', title: ' + model.get('title'));
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```