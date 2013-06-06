JavaScript Developer Guide
=====================================

StackMob's Javascript SDK enables your application to take advantage of StackMob's REST API through convenient AJAX calls.  It's built upon <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>, so you can leverage that framework if you choose.  You don't need to know how Backbone.js works to get started though!  We'll walk you through it.

The <a href="https://developer.stackmob.com/js-sdk/api-docs">StackMob JavaScript SDK API Docs</a> are also available for reference.


## Setup

### Initializing the JS SDK

Setup the JS SDK by including the latest <abbr title="StackMob JavaScript SDK">JS SDK</abbr>.  Check the <a href="https://developer.stackmob.com/js-sdk">StackMob JS SDK page</a> for the latest.

<a href="https://dashboard.stackmob.com/settings">Find your public key on your app dashboard</a> and initialize the <abbr title="StackMob JavaScript SDK">JS SDK</abbr>.

```js,4,5,11,12
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-x.y.z-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
//Make sure that you call `StackMob.init(...)` before using other StackMob calls.  
StackMob.init({
  publicKey: "YOUR PUBLIC KEY", 
  apiVersion: 0
});
/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  //your code goes here (or an external file if you choose)
</script>

</body>
</html>
```

Here's an example of keys in <a href="https://dashboard.stackmob.com/settings" target="_blank">App Settings</a>.

<p class="screenshot"><a href="https://dashboard.stackmob.com/settings" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/setup/app-settings-keys.png" alt="Your App Keys"></a></p>


Notice how we included `jQuery`.  You can also use `Sencha Ext` or `Zepto.js` instead.  Just include the Sencha or Zepto library in place of jQuery.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-general_setup" target="_blank">Setup</a></li>
      </ul>
    </div>
  </div>
</div>


### Defining a Model

Let's say you're creating a simple todo-list app, where you have an object type called `Todo`.  Even before we get to the backend, datastore, and all that good stuff, let's define our object types.

Define your `Todo` class by extending from `StackMob.Model`.  Doing this will give your object its save/fetch abilities (and much more).

<p class="alert">A <code>StackMob.Model</code> is built upon Backbone.js's <code>Model</code> and therefore has all <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">methods of a Backbone.js Model</a>.</p>

```js,15-17
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-x.y.z-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
  StackMob.init({
    publicKey: "YOUR PUBLIC KEY",
    apiVersion: 0
  });

  //Define your Todo class once on the page
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'  //schemaName must be lowercase
  });

/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  // Your code goes here
</script>

</body>
</html>
```

The above `schemaName: 'todo'` tells StackMob to save your `Todo` data under a schema named `todo` on the server side. *We'll get to the server side further down this guide!*


#### set(..)

If you need to modify your object locally (without saving it to the server), you can use `get` and `set`.

You can use the `set(..)` method to pass in or change your local object.  You can call `set` multiple times.

```javascript
<script type="text/javascript">
  var user = new StackMob.User();
  user.set({ username: 'Chuck Norris', password: 'myfists'});
  user.set({
    string: 'abcde',
    int: 10,
    float: 2.5,
    boolean: true,
    arraystring: ['a','b','c'],
    arrayint: [1,2,3],
    arrayfloat: [1.5,2.5,3.5],
    arraybool: [true, false, true]
  });
  user.create();
</script>
```


#### get(..)

You can use `get(..)` to get a field's value on your object:

```javascript
<script type="text/javascript">
  var todo = new Todo({ action: 'Take out the garbage!' });
  todo.get('action'); //Take out the garbage!

</script>
```

<p>StackMob is built on Backbone.js and hence uses the same accessor methods as Backbone Models.  In fact, `StackMob.Model` inherits from `Backbone.Model` so you can use any `Backbone.Model` method.  <a href="http://backbonejs.org/#Model">See the Backbone.Model docs</a>.</p>


#### toJSON()

You can also display the JSON representation of your model by calling the `toJSON()` method.

```javascript
<script type="text/javascript">
  var user = new StackMob.User({ username: 'Chuck Norris' });
  //fetches the user "Chuck Norris" asynchronously
  user.fetch({
    success: function(model) {
      //After StackMob returns "Chuck Norris", print out the result as JSON
      console.debug(model.toJSON());
      /*
       {
        username: 'Chuck Norris',
        weaponofchoice: ...,
        weaknesses: [...],
        strengths: [...],
        createddate: ...,
        lastmoddate: ...
       }
       */
    }
  });
</script>
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.model" target="_blank">StackMob.Model</a></li>
      </ul>
    </div>
  </div>
</div>

### Defining Collections

To work with an array of your objects, define your `Collection`.

```js,5-7
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var Todos = StackMob.Collection.extend({
    model: Todo
  });
```

<p class="alert">A <code>StackMob.Collection</code> is built upon Backbone.js's <code>Collection</code> and hence has all <a href="http://documentcloud.github.com/backbone/#Collection" target="_blank">methods of a Backbone.js Collection</a>.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.collection" target="_blank">StackMob.Collection</a></li>
      </ul>
    </div>
  </div>
</div>


## Datastore

Let's use the JS SDK to persist objects to the datastore and retrieve them.  **You don't have to setup any databases beforehand!**


### Create an Object

Save an instance of your `todo` object to the server.

```javascript
<script type="text/javascript">
  var todo = new Todo({ action: 'Go back and save Marty!', when: 'back to the future!', priority: 1, done: false });
  todo.create({
    success: function(model, result, options) { console.debug(model.toJSON()); },
    error: function(model, error, options) {}
  });
</script>
```

That's it!  That fires off an AJAX call to StackMob which saves your object.  StackMob will **automatically create your schema** for you if it doesn't already exist.  

Here's how it'll look on the server:

<p class="screenshot">
  <a href="https://dashboard.stackmob.com/schemas/edit/todo" target="_blank"><img border="0" src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/schemas-todo-basic-fields.png" alt-""/></a>
</p>

And the data:

<p class="screenshot">
  <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank"><img border="0" src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/objectbrowser-todo-basic.png" alt=""/></a>
</p>

StackMob automatically generates some fields for you too.

* `todo_id` (the primary key) - primary keys will have the format of `[schemaName]_id`
* `createddate` - the created date in milliseconds
* `lastmoddate` - the last modified date in milliseconds
* `sm_owner` - who created the object (we'll get into this later!)


If you recall from earlier, you defined your `Todo` object to bind to the `todo` schema:

```scala
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
```

This tells StackMob to save your object in the `todo` schema.

<p class="alert">
  If the schema doesn't already exist on the server, StackMob will auto create your schema when you make a `create` call. (This only occurs in the development environment.)
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-create" target="_blank">StackMob.Model#create</a></li>
      </ul>
    </div>
  </div>
</div>


### Asynchronous Calls

Notice that there are `success` and `error` callback functions above.  Most StackMob calls are done asynchronously via an AJAX call. This means that your code in the browser continues to run while StackMob continues to process your request.  `success` and `error` are guaranteed to only execute **after** the AJAX call has returned.

For example, let's try getting the JSON representation of an object after we've created it.

```js
var todo = new Todo({ ... });
todo.create(); //an AJAX call is fired to StackMob
todo.toJSON(); //this may not work because "create" may still be waiting for AJAX request to complete!
```

Your code should be inside the success/error callbacks.

```js
var todo = new Todo({ ... });
todo.create({
  success: function(model, result, options) { console.debug(todo.toJSON()); },  //this is guaranteed to run after the AJAX call is complete
  error: function(model, result, options) {}
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-callbacks_and_options" target="_blank">Callbacks and Options</a></li>
      </ul>
    </div>
  </div>
</div>


### Read an Object

To fetch your `todo` object, just specify the primary key and run `fetch`.  Again, the primary key takes the form of `[schemaName]_id`.

```js
var todo = new Todo({ todo_id: '1234' });
todo.fetch({
  success: function(model, result, options) {
    console.debug(model.toJSON());
  }, error: function(model, result, options) {}
});
```

Your object will now be populated.  The `model` above is a reference to your object.  Calling `todo.toJSON()` is equivalent.

```js,3
todo.fetch({
  success: function(model, result, options) {
    console.debug(todo.toJSON());
  }, error: function(model, result, options) {}
});
```

<p class="alert">
  Don't forget that StackMob calls are asynchronous, so you'll need to operate on your objects in your success/error callbacks.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-fetch" target="_blank">StackMob.Model#fetch</a></li>
      </ul>
    </div>
  </div>
</div>


### Update an Object

You can edit existing objects easily.  Let's update `todo` object `1234` with a new field: `done` and let's mark it as `true`.

```javascript
var todo = new Todo({ todo_id: '1234' });
todo.save({ done: true }, {
  success: function(model, result, options) { console.debug(model.toJSON()); }
  error: function(model, result, options) {}
});
```

<p class="alert">
  If you save a <b>field</b> that previously didn't exist, StackMob will add that to your schema automatically (only in the development environment).
</p>

You can also set fields and save things later.

```javascript
var todo = new Todo({ todo_id: '1234' });

todo.set({ done: true });

todo.save({
  success: function(model, result, options) { console.debug(model.toJSON()); }
  error: function(model, result, options) {}
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-save" target="_blank">StackMob.Model#save</a></li>
      </ul>
    </div>
  </div>
</div>


### Delete an Object

Let's now delete your object.


```javascript
var todo = new Todo({ todo_id: '1234' });
user.destroy({
  success: function(model, result, options) { console.debug(model.toJSON()); },
  error: function(model, result, options) {}
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-destroy" target="_blank">StackMob.Model#destroy</a></li>
      </ul>
    </div>
  </div>
</div>


### Arrays

You can save arrays.

```js,3
var todo = new Todo();
todo.save({
  subtasks: ['do A', 'do B']
}, {
  success: ...
  error: ...
});
```

Arrays are special in that if you have two users saving to the same array at the same time, they could overwrite each others changes.

e.g. John saves `['do A', 'do B']` and Jill saves `['do C']`.  Whoever calls `save` last will "win".

But sometimes you want to append to an array.  StackMob provides ways of *appending* to arrays so that the end result will be `['do A', 'do B', 'do C']`, even if multiple users are operating on that field.

```javascript
var todo = new Todo({ todo_id: '12345' });
todo.appendAndSave('subtasks', ['do C']);
```

If you have multiple users appending to the same array concurrently, StackMob makes sure that each append is respected (and not overwritten as you would run into if you did a `save` on the object).

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-appendandsave" target="_blank">StackMob.Model#appendAndSave</a></li>
      </ul>
    </div>
  </div>
</div>


### Deleting from an array

Now let's safely delete from an array.

```javascript
var todo = new Todo({ todo_id: '12345' });
todo.deleteAndSave('subtasks', ['rinse and repeat']);
```

As with `appendAndSave`, concurrency issues are handled appropriately.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-deleteandsave" target="_blank">StackMob.Model#deleteAndSave</a></li>
      </ul>
    </div>
  </div>
</div>


## Queries

To query objects by field parameters, paginate, sort by, and more, use `StackMob.Collection.Query` in conjunction with your `Collection`.  In our case, that'll be `Todos`.  Let's get the first 5 `high` priority todo items, ordered by `createddate`.

```javascript
var q = new StackMob.Collection.Query();
q.equals('priority', 'high').orderAsc('createddate').notEquals('done', true); //you can chain query parameters
q.setRange(0,4); //or add them individually

var todos = new Todos();
todos.query(q, {
  success: function(collection) {
    console.debug(collection.toJSON());
  }
});   
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.collection.query" target="_blank">StackMob.Collection.Query</a></li>
      </ul>
    </div>
  </div>
</div>

### Comparison

Query by greater than, greater than or equal too, less than...

```js
var fiveDaysAgo = (new Date()).getTime() - (5 * 24 * 60 * 60 * 1000);
var twoDaysAgo = (new Date()).getTime() - (2 * 24 * 60 * 60 * 1000)

var q = new StackMob.Collection.Query();
q.gt('createddate', fiveDaysAgo); //created date greater than 5 days ago
q.lte('lastmoddate', twoDaysAgo); //last modified less than or equal to 2 days ago

var todos = new Todos();
todos.query(q, ...);
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-lt__less_than_" target="_blank">StackMob.Collection.Query#lt,lte,gt,gte</a></li>
      </ul>
    </div>
  </div>
</div>


### Relationship and Arrays

Query for any `todo` object with a `subtask` of `do C`.

```js
var q = new StackMob.Collection.Query();
q.mustBeOneOf('subtasks', ['do C']);
var todos = new Todos();
todos.query(q, ...);
```

Relationships are lists of primary keys, so it works on relationships as well (more info below).

```js
var q = new StackMob.Collection.Query();
q.mustBeOneOf('friends', ['john']);

...

todos.query(q);
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-mustbeoneof" target="_blank">StackMob.Collection.Query#mustBeOneOf</a></li>
      </ul>
    </div>
  </div>
</div>


### Pagination

Return "page 2" of results, assuming 5 per page.

```js
var q = new StackMob.Collection.Query();
q.setRange(5,9);

...

todos.query(q);
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-setrange" target="_blank">StackMob.Collection.Query#setRange</a></li>
      </ul>
    </div>
  </div>
</div>


### Select Fields

Only return back a subset of fields, reducing the payload.

```js
var q = new StackMob.Collection.Query();
q.select('subtasks');

...

todos.query(q);
```

This will return a smaller `todo` JSON from the server:

```js
{
  todo_id: '1234',
  subtasks: ['do A', 'do B']
}
```

StackMob always returns the primary key, but no other fields are returned.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-select" target="_blank">StackMob.Collection.Query#select</a></li>
      </ul>
    </div>
  </div>
</div>


## Relationships

Associate two separate objects together in a relationship.

Let's give a `user` object several `todo` items.

[DOC ON RELATIONSHIPS/SCREENSHOTS]


### Adding Related Objects

You can create objects and add them to a related parent object in one swoop.

Let's assign some more chores for `Marty`. 

```javascript
var chore1 = new Todo({ action: 'Save Doc!' });
var chore2 = new Todo({ action: 'Save Mom and Dad!' });
var chore3 = new Todo({ action: 'Have Biff wash my car.' });

var user = new StackMob.User({ username: 'Marty' });
user.addRelationship('chores', [chore1, chore2, chore3], { ..success... });
```

In a single call, the chores will be created against the `todo` schema, and the `user` will be updated as well.  What `Marty`'s JSON looks like:

```js
{
 username: 'Marty',
 chores: [ 
    '1', 
    '2', 
    '3' ], //these are "todo_ids" of the chores 
 'createddate': ...,
 'lastmoddate': ...
}
```

Meanwhile querying `todo` will return

```js
var todos = new Todos();
todos.fetch();

...


[
  { todo_id: '1', action: 'Save Doc!'},
  { todo_id: '2', action: 'Save Mom and Dad!'},
  { todo_id: '3', action: 'Have Biff wash my car.!'},
]
```

<p class="alert">
  StackMob handles relationships by reference.  StackMob does not embed JSON within JSON.  There's only one instance of it in the datastore, making your data more easily editable and flexible.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-addrelationship" target="_blank">StackMob.Model#addRelationship</a></li>
      </ul>
    </div>
  </div>
</div>


### Fetching Related Objects (Join)

Though StackMob keeps related objects by ID, you can get the full JSON for related objects from the server easily with `fetchExpanded`.

```js,2
var user = new StackMob.User({ username: 'Marty' });
user.fetchExpanded(1);

//user.toJSON()
{
 username: 'Marty',
 chores: [
  {
    todo_id: '1',
    action: 'Save Doc!',
    createddate: ...,
    lastmoddate: ... 
  },
  {
    todo_id: '2',
    action: 'Save Mom and Dad!',
    createddate: ...,
    lastmoddate: ... 
  },
  {
    todo_id: '3',
    action: 'Have Biff wash my car.',
    createddate: ...,
    lastmoddate: ... 
  }
 ], 
 'createddate': ...,
 'lastmoddate': ...
}
```

Related objects can have related objects themselves, so you can call `fetchExpanded(2)` or `fetchExpanded(3)` as well to expand deeper.

You can also specify `expand` on a `Query`.

```js
var q = new StackMob.Collection.Query();
q.setExpand(3);
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-fetchexpanded" target="_blank">StackMob.Model#fetchExpanded (for a single model)</a></li>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-setexpand" target="_blank">StackMob.Collection.Query#setExpand (for a collection query)</a></li>
      </ul>
    </div>
  </div>
</div>


#### Selecting Related Fields

You can limited the fields returned for related objects.

Only get the `action` from related `todo` objects, represented by the `chores` field:

```js
var todos = new Todos();

var q = new StackMob.Collection.Query();
q.select('chores').select('chores.action').setExpand(1);

todos.query(q, ...)

//todos.toJSON()
{
  username: 'Marty',
  chores: [
    {
      todo_id: 1,
      action: 'Save Doc!'
    },
    ...
  ]
}
```

<p class="alert">
  Select fields to improve performance by reducing the payload that gets sent back.  Particularly useful when you have cyclical references.  <b>StackMob always returns the primary key</b>, even when not specified.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-select" target="_blank">StackMob.Collection.Query#select</a></li>
      </ul>
    </div>
  </div>
</div>


### Decoupling Related Objects

You can decouple the relationship between two objects.  Remove chore `1` from the `Marty`'s list.  The todo object `1` remains in the datastore.  Use `StackMob.SOFT_DELETE`.

```js,3
var user = new StackMob.User({ username: 'Marty' });
user.deleteAndSave('chores', ['1'], 
  StackMob.SOFT_DELETE, 
  { success: ..., error: ... }); 
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-deleteandsave" target="_blank">StackMob.Model#deleteAndSave</a></li>
      </ul>
    </div>
  </div>
</div>


#### Delete Related Objects

But you may want to delete a relationship **and** remove the actual related object from the database.  Use `StackMob.HARD_DELETE`.

```js,3
var user = new StackMob.User({ username: 'Marty' });
user.deleteAndSave('chores', ['1'], 
  StackMob.HARD_DELETE, 
  { success: ..., error: ... }); 
</script>
```

Todo object `1` is removed from the datastore completely.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-deleteandsave" target="_blank">StackMob.Model#deleteAndSave</a></li>
      </ul>
    </div>
  </div>
</div>

## User Authentication

StackMob also gives you a way to authenticate your users.  The JS SDK uses OAuth 2.0 to login. It uses your `user` schema to perform login.  Authentication is made available with the `StackMob.User` model.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.user" target="_blank">StackMob.User</a></li>
      </ul>
    </div>
  </div>
</div>


### Creating a User

StackMob provides a `StackMob.User` class for you out of the box.  Let's create a `StackMob.User` below, saving it to the server.

```js
var user = new StackMob.User({ username: 'Bill Watterson', password: 'weirdosfromanotherplanet', profession: 'cartoonist'  });
user.create({
  success: function(model, result, options) {},
  error: function(model, result, options) {}
});
```

`StackMob.User` is provided with the JS SDK.  It has special authentication methods built in.

<p class="alert"><code>username</code> is the default primary key for <code>StackMob.User</code> objects.  <code>password</code> is a special field that gets encrypted on the server.</p>


### Fetching a User

Fetching a user is simple.  Like other objects, just provide the primary key.


```javascript
var user = new StackMob.User({ username: 'Bill Watterson' });
//fetches the user "Bill Watterson" asynchronously
user.fetch({
  success: function(model, result, options) {}
});
```


### Declaring your own User Object type

Are you using a schema besides `user`?  Maybe you renamed your `username` and `password` fields during the schema creation process.  Declare a new User type.

```js
var Customer = StackMob.User.extend({
  schemaName: 'customer',
  loginField: 'email',
  passwordField: 'pass'
});

var c = new Customer({
  email: 'billybob@email.com',
  passwordField: 'yeehaw'
});
c.create(); //saves to "customer"
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-login_-_with_custom_schema" target="_blank">StackMob.User Custom Schema and Fields</a></li>
      </ul>
    </div>
  </div>
</div>

### Login

Log in your user.

```js
var user = new StackMob.User({ username: 'chuck norris', password: 'myfists' });
user.login(false, {
  success: function(model, result, options) {
    //if successfully logged in, StackMob returns the full user object to you
  },
  error: function(model, result, options) {
    console.error(result); //or print out the error
  }
});
```

You can also stay logged in beyond the current session by flagging `true`:

```js,1
user.login(true, {
  success: ...
  error: ...
})
```

Your user will be logged in for an hour.

* Logging in from another device will invalidate the session on other devices.

<p class="alert">
  StackMob's uses OAuth 2.0 authentication, the industry standard.  When logging in, users are issued an accessToken that will be used to sign their requests to identify them.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-login" target="_blank">StackMob.User#login</a></li>
      </ul>
    </div>
  </div>
</div>

### Logout

Logging out is easy.

```javascript
var user = new StackMob.User();  //no username necessary, since only 1 user is logged in on the device at a time
user.logout();  
```


### Checking Login Status

Check to see if a user is logged in.  These methods are asynchronous because they also check the server in some instances.

```js

StackMob.isUserLoggedIn('chuck norris', {
  yes: function() { /* if yes */ },
  no: function() { /* if no */ }
});

StackMob.isLoggedIn({
  yes: function() { /* if yes */ },
  no: function() { /* if no */ }
});

var user = new StackMob.User({ username: 'chuck norris' });
user.isLoggedIn({
  yes: function() { /* if yes */ },
  no: function() { /* if no */ }
});

```

<p class="alert">For optimization, the methods only check the server if we've locally determined that your access token is expired.  It does so in case you were issued refresh tokens that would reopen your user session.  Otherwise, "yes" is called immediately if local tokens exist.</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-isloggedin_-_stackmob" target="_blank">StackMob#isLoggedIn</a></li>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-isuserloggedin" target="_blank">StackMob#isUserLoggedIn</a></li>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-isloggedin_-_stackmob.user" target="_blank">StackMob.User#isLoggedIn</a></li>
      </ul>
    </div>
  </div>
</div>


### Get Logged In User

Get the logged in user.  This is asynchronous, as it may check the server.

```js
StackMob.getLoggedInUser({
  success: function(username) {
    //username is a string if logged in, null if not.
  }
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-getloggedinuser" target="_blank">StackMob#getLoggedInUser</a></li>
      </ul>
    </div>
  </div>
</div>


### Change Password

```js
var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
user.login();


user.resetPassword('myfists', 'mynewandimprovedfists', {
  success: function(..) {},
  error: function(..) {}
}); //changes password from 'myfists' to 'mynewandimprovedfists'
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-resetpassword" target="_blank">StackMob.User#resetPassword</a></li>
      </ul>
    </div>
  </div>
</div>

### Password Recovery

If your user has forgotten his or her password, they can request that an email with a temporary password be sent to an email address.  That email address should be a field in the user schema.  You'll need to specify which field represents the email address when defining your User schema.

<p class="screenshot">
  <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/forgotpassword/forgotpassword-link.png" alt="Select your email field in the User Schema"/></a>
</p>

```js
var user = new StackMob.User({ username: 'Chuck Norris' });
//assume StackMob.User has an "email" field, and Chuck Norris's email is "chucknorris@somewhere.com"
user.forgotPassword({
  success: function(...) {},
  error: function(...) {}
}); //tells StackMob to send an email to Chuck Norris at "chucknorris@somewhere.com"
```

The user will receive the new password in their email.  You should then have them login with the temporary password and provide a new one.

```js,6
var user = new StackMob.User({ username: 'Chuck Norris' });
user.forgotPassword(...); 

//after checking getting his temp password in his email...

user.loginWithTempAndSetNewPassword('temporary password from StackMob email', 'mysuperfists', true, {
  success: function(..) {},
  error: function(..) {}
});
```

As with `login`, you can set whether or not to stay logged in.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-forgotpassword" target="_blank">StackMob.User#forgotPassword</a></li>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-loginwithtempandsetnewpassword" target="_blank">StackMob.User#loginWithTempAndSetNewPassword</a></li>
      </ul>
    </div>
  </div>
</div>

## Facebook

You can log in Facebook users into your StackMob app.  After using the Facebook JavaScript SDK to login, simply pass the Facebook access token into the login method.  StackMob will log the user in.  If the user doesn't exist yet, StackMob will create an instance for you in the datastore.

```js
FB.login(function(response) {
  if (response.authResponse) {

    var accessToken = response.authResponse.accessToken;
    var user = new StackMob.User();
    user.loginWithFacebookAutoCreate(accessToken, true); //true, stay logged in.

  } else {
    console.log('User cancelled login or did not fully authorize.');
  }
}, {scope: 'email'});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-loginwithfacebookautocreate" target="_blank">StackMob.User#loginWithFacebookAutoCreate</a></li>
      </ul>
    </div>
  </div>
</div>

## Access Controls

StackMob provides you the ability to lock down access to your data with Access Controls.  You can disallow access to Create, Read, Update or Delete.  With user authentication, you can even restrict permissions at a user level.

Here, we set it so that only logged in users can create objects for this schema.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/Scrumptious/dashboard-acl.png" alt=""/>

So now if you call `todos.fetch(..)`, you'll get a 401 Unauthorized error if you aren't logged in.  Because the logic is handled server-side, nobody can mess with your control logic even if they view the client side code.

<a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">Read about all the Access Control options.</a> Find which one works best for your use case.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">Access Control Options</a></li>
      </ul>
    </div>
  </div>
</div>

## Files

You can upload files to StackMob.  The files are saved to your Amazon S3 account, which you link with StackMob.  This'll allow you to more easily manage your files and retain content.

<a href="https://developer.stackmob.com/modules/s3/docs">Link StackMob with your S3 account and create a `binary` field on your schema.</a>

A file is represented as a `Binary` file type field on your schema.  To save the file, you'll call `setBinaryFile` with three parameters:

* the file, Base64 encoded as a string
* the file name
* the file type (image/png)

The code:

```js
var base64Content = //the file, base64 encoded as a string
var fileName = //
var fileType = theFile.type;

user.setBinaryFile('profilepic', fileName, fileType, base64Content);
```

Let's take an expanded example and get a file from the local filesystem with the HTML5 FileReader API:

```js
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>
 
<script type="text/javascript">
/* <![CDATA[ */
    StackMob.init({
        publicKey: 'your_public_key',
    apiVersion: 0
  });
/* ]]> */
</script>
 
</head>
<body>
 
<table>
  <tr>
    <td>File to Encode:</td>
    <td><input type="file" id="files" name="files[]" multiple /></td>
  </tr>
</table>
 
<script type="text/javascript">
  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });
 
  var todoInstance = new Todo();
  var fieldname = 'picture'; //name of binary field that you created in your schema
  
  function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object
 
    // Loop through the FileList
    for (var i = 0, f; f = files[i]; i++) {
 
      var reader = new FileReader();
 
      // Closure to capture the file information
      reader.onload = (function(theFile) {
        return function(e) {
 
          /*
            e.target.result will return "data:image/jpeg;base64,[base64 encoded data]...".
            We only want the "[base64 encoded data] portion, so strip out the first part
          */
          var base64Content = e.target.result.substring(e.target.result.indexOf(',') + 1, e.target.result.length);
          var fileName = theFile.name;
          var fileType = theFile.type;
 
          todoInstance.setBinaryFile(fieldname, fileName, fileType, base64Content);
          todoInstance.save();
 
        };
      })(f);
 
      // Read in the file as a data URL
      fileContent = reader.readAsDataURL(f);
 
    }
  }
 
  document.getElementById('files').addEventListener('change', handleFileSelect, false);
 
</script>
 
</body>
</html>
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-setbinaryfile" target="_blank">StackMob.Model#setBinaryFile</a></li>
      </ul>
    </div>
  </div>
</div>


## Geolocation

StackMob supports geolocations, allowing you to save and search geo data.  Geolocations are saved to a `geopoint` field type on your schema.  Add a 


### Saving Geopoints

Let's save a geopoint to `place`.  We'll use the `StackMob.GeoPoint` object to help with things.  It's a helper object that will convert the geopoint data to the JSON format we need via `toJSON`.

```js
var Place = new StackMob.Model.extend({ schemaName: 'place' }); 
 
var latlon = new StackMob.GeoPoint(37.772161,-122.406443); //lat, lon
var stackmob_office = new Place({ location: latlon.toJSON() }); //notice "toJSON()"
stackmob_office.create({
  success: function(..) {},
  error: function(..) {}
});
```

That's it!  Check out the dashboard for your new point.

[SCREENSHOT OF OBJECT BROWSER]

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.geopoint" target="_blank">StackMob.GeoPoint</a></li>
      </ul>
    </div>
  </div>
</div>


### Querying Geopoints

To query for geopoints near a particular location, you can use several `StackMob.Collection.Query` additions for geopoints.  You can use:

* `mustBeNearMi` - returns locations near to you with the distance from point (in miles)
* `isWithinMi` - returns locations near you without the distance from point (in miles)
* `isWithinBox` - returns locations within a rectangle

These queries are generally in radians, but we've provided helper methods to search by `miles` or `kilometers`.

Let's search for places 1 mile near StackMob.

```js,11
var Place = StackMob.Model.extend({ schemaName: 'place' });
var Places = StackMob.Collection.extend({ model: Place });
 
var latlon = new StackMob.GeoPoint(37.772161, -122.406443);
 
var q = new StackMob.Collection.Query();
q.mustBeNearMi('location', latlon, 1);
 
var placesContainer = new Places();
placesContainer.query(q, {
  success: function(model) {
    console.debug(model);
  },
  error: function(model, response) {
    console.debug(response);
  }
});
```

Now let's query for places within a search box, represented with two opposite corners of the rectangle as geopoints.

```js
var Place = StackMob.Model.extend({ schemaName: 'place' });
var Places = StackMob.Collection.extend({ model: Place });

 
var q = new StackMob.Collection.Query();
q.isWithinBox('location', new StackMob.GeoPoint(37.77493,-122.419416), new StackMob.GeoPoint(37.783367,-122.408579));
 
var places = new Attractions();
places.query(q, {
  success: function(collection) {
      console.debug(collection.toJSON());
  }
});
```

See more about geolocation data in the resources below.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-stackmob.collection.query_with_stackmob.geopoint" target="_blank">StackMob.Collection.Query: mustBeNear,isWithin,isWithinBox (radians, mi, km)</a></li>
      </ul>
    </div>
  </div>
</div>


## Custom Code

With the Custom Code module, StackMob lets you write code that runs and executes on the server.  Sending batch push messages?  Running some jobs?  Write it in Custom Code and call it from the SDK or anything that can hit a REST API.  You can define custom JSON responses so that your server can also notify the client of the result.

```js
StackMob.customcode('helloworld', 
  { who: "Kodi" }, //method parameters
  'GET', //skip third param for default 'GET'
  {
      success: function(result) {
          console.debug(result); //prints out the returned JSON your custom code specifies
          /*
           {
              msg: "Hello World, Kodi!"
           }
           */
      }
  } 
);
```

The above sends a `GET` request to `https://api.stackmob.com/helloworld?who=Kodi`.

Read more about <a href="/customcode-sdk/developer-guide" target="_blank">StackMob Custom Code</a>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/sdks/js/api#a-customcode" target="_blank">StackMob#customcode (StackMob#cc)</a></li>
      </ul>
    </div>
  </div>
</div>


## Deploy

You're done developing your app!  You've been working in StackMob development environment and you now want to get everything to the production environment.  StackMob let's you do that easily.  Let's go through what you'll need to do.

1.  Check your CORS settings
2.  Deploy your API
3.  Deploy HTML5 (only if using StackMob's HTML5 hosting module)

### CORS Settings

When deploying to production, you'll need to update your <a href="https://dashboard.stackmob.com/module/cors/settings" target="_blank">CORS settings</a> and whitelist the domains you want to receive StackMob API calls from.  By default, your development environment allows all domains.

Why do you need this?  Browsers typically only allow AJAX calls to fire against the same origin/domain.  This means that if you're on `https://www.mysite.com`, browsers won't allow AJAX calls to go to `https://api.stackmob.com`.  But with CORS (cross origin resource sharing), modern browsers can.

Perhaps you've seen the cross-domain call error before:

<p class="screenshot"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/cors/modules-cors-origin-error.png" alt=""/></p>

This is a result of not adding `http://fiddle.jshell.net` to the CORS list.

Again, the **development environment** is defaulted to allow any domain.  You can configure your accepted development and production domains in the <a href="https://dashboard.stackmob.com/module/cors/settings" target="_blank">CORS module settings</a>.  Most importantly, you'll need to whitelist your production domains.  The protocol and port matter.

### API

StackMob gives you separate development and production environments so that you can keep your test data and custom code separate from your production set.  Deploying your API is a *critical* step you need to do when deploying.  StackMob provides a simple UI to help you do this easily.

Read about how to <a href="https://developer.stackmob.com/" target="_blank">Deploy your API</a>

<p class="screenshot"><a href="https://dashboard.stackmob.com/deploy" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/apiversions/modules-apiversions-deploy.png" alt=""/></a></p>

After deploying, you would point your JS SDK at your production API Version - in this case `1`:

```js,3
StackMob.init({
  publicKey: ...,
  apiVersion: 1
});
```

That's it!  With your server rolled out to API Version 1, you can point your JS SDK at your production server and start making calls.  Your production environment has a different database than your development one, so you'll be able to continue developing in API Version 0 without affecting your production customers.

<p class="alert">
  You can have <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">multiple production API Versions</a> so that you can deploy multiple clients pointing at older versions of your API.  StackMob's <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">Multiple API Versions module</a> makes backwards compatibility easy.
</p>


### HTML5

If you're using StackMob's HTML5 hosting service, you'll also need to <a href="https://developer.stackmob.com/" target="_blank">deploy your HTML files to production</a>.

<p class="screenshot"><a href="https://dashboard.stackmob.com/deploy" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/html5/modules-html5-deploy.png" alt=""/></a></p>