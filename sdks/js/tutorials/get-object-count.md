<h3>Objectives</h3>
Query some objects and count the result

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-query_-_stackmob.collection" target="_blank">query</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-count_-_stackmob.collection" target="_blank">count</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.collection.query" target="_blank">List of Available Queries</a>

<h2>Read a Specific Object</h2>

Let's see how many `Todo` objects with `title` `Send Data to StackMob!` exist.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var Todos = StackMob.Collection.extend({ model: Todo });
var myTodos = new Todos();
var q = new StackMob.Collection.Query();
q.equals('title', 'Send Data to StackMob!');
myTodos.count(q, {
	success: function(count) {
		console.debug(count);
	},
	error: function(model, response) {
		console.debug(response);
	}
})
```