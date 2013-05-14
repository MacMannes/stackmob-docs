<h3>Objectives</h3>
Read a specific object and Filter/sort multiple objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is at least one `Todo` object with `Send Data to StackMob!` as its `title` (add using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-an-Object" target="_blank">create tutorial</a>)

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-query_-_stackmob.collection" target="_blank">query</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.collection.query" target="_blank">List of Available Queries</a>

<h2>Read a Specific Object</h2>

Let's try to read specific `Todo` object which title is `Send Data to StackMob!`.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var Todos = StackMob.Collection.extend({ model: Todo });
var myTodos = new Todos();
var q = new StackMob.Collection.Query();
q.equals('title', 'Send Data to StackMob!');
myTodos.query(q, {
	success: function(model) {
		console.debug(model.toJSON()); //JSON array of matching Todo objects
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

If you want all the entries, you can omit line 5.

<h2>Filter and sort multiple objects</h2>

You can also query for multiple objects that match criteria.  Let's search for `Todo` objects in the `category` of `homework` and sort by title.  We'll get the first 10 results.

```js
//Assume Todo (StackMob.Model) and Todos (StackMob.Collection) has been declared above.

var myTodos = new Todos();
var q = new StackMob.Collection.Query();
q.equals('category', 'homework');
q.orderAsc('title'); //sort by title in ascending order
q.setRange(0,9); //get the first 10.  second 10 would be setRange(10,19)
myTodos.query(q, {
	success: function(model) {
		console.debug(model.toJSON()); //JSON array of matching Todo objects
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

Check out the [query docs](https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.collection.query) for even more ways to filter and sort.

