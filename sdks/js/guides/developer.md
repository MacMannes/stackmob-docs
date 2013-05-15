Developer Guide
======================================================

StackMob's Javascript SDK enables your application to take advantage of StackMob's REST API through convenient AJAX calls.  It's built upon <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>, so you can leverage that framework if you choose.  You don't need to know how Backbone.js works to get started though!  We'll walk you through it.

The <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs">StackMob JavaScript SDK API Docs</a> are also available for reference.


# Setup

## Initializing the JS SDK

Setup the JS SDK by including the latest <abbr title="StackMob JavaScript SDK">JS SDK</abbr>.  Check the <a href="https://developer.stackmob.com/stackmob-js-sdk">StackMob JS SDK page</a> for the latest.

[SCREENSHOT OF DASHBOARD AND KEYS?]

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

Notice how we included `jQuery`.  You can also use `Sencha Ext` or `Zepto.js` instead.  Just include the Sencha or Zepto library in place of jQuery.


## Defining a Model

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


### set(..)

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


### get(..)

You can use `get(..)` to get a field's value on your object:

```javascript
<script type="text/javascript">
  var todo = new Todo({ action: 'Take out the garbage!' });
  todo.get('action'); //Take out the garbage!

</script>
```

<p>StackMob is built on Backbone.js and hence uses the same accessor methods as Backbone Models.  In fact, `StackMob.Model` inherits from `Backbone.Model` so you can use any `Backbone.Model` method.  <a href="http://backbonejs.org/#Model">See the Backbone.Model docs</a>.</p>


### toJSON()

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


## Defining Collections

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


# Datastore

Let's use the JS SDK to persist objects to the datastore and retrieve them.  **You don't have to setup any databases beforehand!**


## Creating Objects

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

[SCREENSHOT OF DATA BROWSER]

And 

[SCREENSHOT OF SCHEMA LIST]

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


## Asynchronous Calls

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


## Reading Objects

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


## Updating Objects

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


## Deleting Objects

Let's now delete your object.


```javascript
var todo = new Todo({ todo_id: '1234' });
user.destroy({
  success: function(model, result, options) { console.debug(model.toJSON()); },
  error: function(model, result, options) {}
});
```


## Arrays

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


## Deleting from an array

Now let's safely delete from an array.

```javascript
var todo = new Todo({ todo_id: '12345' });
todo.deleteAndSave('subtasks', ['rinse and repeat']);
```

As with `appendAndSave`, concurrency issues are handled appropriately.


# Queries

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


## Comparison

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


## Relationship and Arrays

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


## Pagination

Return "page 2" of results, assuming 5 per page.

```js
var q = new StackMob.Collection.Query();
q.setRange(5,9);

...

todos.query(q);
```


## Select Fields

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


# Relationships

Associate two separate objects together in a relationship.

Let's give a `user` object several `todo` items.

[DOC ON RELATIONSHIPS/SCREENSHOTS]


## Adding Related Objects

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


## Fetching Related Objects (Join)

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


### Selecting Related Fields

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


## Decoupling Related Objects

You can decouple the relationship between two objects.  Remove chore `1` from the `Marty`'s list.  The todo object `1` remains in the datastore.

```javascript
var user = new StackMob.User({ username: 'Marty' });
user.deleteAndSave('chores', ['1'], StackMob.SOFT_DELETE, { success: ..., error: ... }); 
```


### Delete Related Objects

But you may want to delete a relationship **and** remove the actual related object from the database.

```javascript
var user = new StackMob.User({ username: 'Marty' });
user.deleteAndSave('chores', ['1'], StackMob.HARD_DELETE, { success: ..., error: ... }); 
</script>
```

Todo object `1` is removed from the datastore completely.


# User Authentication

StackMob also gives you a way to authenticate your users.  The JS SDK uses OAuth 2.0 to login. It uses your `user` schema to perform login.


## Creating a User

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


## Fetching a User

Fetching a user is simple.  Like other objects, just provide the primary key.


```javascript
var user = new StackMob.User({ username: 'Bill Watterson' });
//fetches the user "Bill Watterson" asynchronously
user.fetch({
  success: function(model, result, options) {}
});
```


## Declaring your own User Object type

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


## Login

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

* Logging in from another device will invalidate the session on other devices.

<p class="alert">
  StackMob's uses OAuth 2.0 authentication, the industry standard.  When logging in, users are issued an accessToken that will be used to sign their requests to identify them.
</p>


## Logout

Logging out is easy.

```javascript
var user = new StackMob.User();  //no username necessary, since only 1 user is logged in on the device at a time
user.logout();  
```


## Checking Login Status

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


## Get Logged In User

Get the logged in user.  This is asynchronous, as it may check the server.

```js
StackMob.getLoggedInUser({
  success: function(username) {
    //username is a string if logged in, null if not.
  }
});
```


## Change Password


## Password Recovery


# Access Controls


# Files


# Geolocation


# Facebook


# Twitter


# Cross Domain AJAX


# Deploy


Check the <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" target="_blank">Javascript SDK API documentation page</a> for more!

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-forgotpassword" target="_blank">Forgot Password/Reset Password</a>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-createuserwithfacebook" target="_blank">Facebook Integration</a>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.geopoint" target="_blank">Geo Spatial Queries</a>

In the meantime, feel free to visit the Javascript API Docs for more info and examples. <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" class="button blue">Read StackMob Javascript API Docs</a>