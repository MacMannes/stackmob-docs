# StackMob Javascript SDK Developer Guide

StackMob's Javascript SDK enables your application to take advantage of StackMob's REST API through convenient AJAX calls.  It's built upon <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>, so you can leverage that framework if you choose.  You don't need to know how Backbone.js works to get started though!  We'll walk you through it.

The <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs">StackMob JavaScript SDK API Docs</a> are also available for reference.

# SDK Setup

Setup the JS SDK by including the latest <abbr title="StackMob JavaScript SDK">JS SDK</abbr>.  Check the <a href="https://developer.stackmob.com/stackmob-js-sdk">StackMob JS SDK page</a> for the latest.

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

# Objects


In the following examples, let's pretend you're creating a simple todo-list app, where you have an object type called `Todo`.

## Define your example Todo Class

We'll define your `Todo` class by extending from `StackMob.Model`.  `StackMob.Model` will give your object its save/fetch abilities (and much more).

<p class="alert">A <code>StackMob.Model</code> is built upon Backbone.js's <code>Model</code> and hence has all <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">methods of a Backbone.js Model</a>.</p>

<span/>


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
  //your code goes here
</script>

</body>
</html>
```

The above `schemaName: 'todo'` tells StackMob to save your `Todo` data under a schema named `todo`.

## Setting Fields


If you need to modify your object locally (without saving it to the server), you can use `get` and `set`.

### set(..)

You can use the `set(..)` method to pass in or change your local JSON.  You can call `set` multiple times.

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

You can use `get(..)` to get the value of your object's field:

```javascript
<script type="text/javascript">
  var user = new StackMob.User({ username: 'Chuck Norris' });
  user.fetch({
    success: function(model) {
      console.debug(model.get('weapon')); //"NunChucks"
    }
  });

</script>
```

<p>StackMob is built on Backbone.js and hence uses the same accessor methods as Backbone Models.  In fact, `StackMob.Model` inherits from `Backbone.Model` so you can practically use any `Backbone.Model` method.  <a href="http://backbonejs.org/#Model">See the Backbone.Model docs</a>.</p>

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

# Datastore

Let's use the JS SDK to persist objects to the datastore and retrieve them.

## Creating Objects

Let's now create an instance of your `todo` object.

```javascript
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

  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  var todo = new Todo({ action: 'Go back and save Marty!', when: 'back to the future!', priority: 'high' });
  todo.create({
    success: function(model, result, options) { console.debug(model.toJSON()); },
    error: function(model, error, options) {}
  });
</script>

</body>
</html>
```

You've now created your own `Todo` class and saved a `Todo` object!  

StackMob will automatically create your schema for you if it doesn't already exist.  StackMob will also generate several fields on your object as well:

* `todo_id` (the primary key) - primary keys will have the format of `[schemaName]_id`
* `createddate` - the created date in milliseconds
* `lastmoddate` - the last modified date in milliseconds
* `sm_owner` - who created the object (we'll get into this later!)


Notice in `line 19`, you've defined the `schemaName: todo`.  This tells StackMob to save your object in the `todo` schema.  Making the call above, you'll see on your <a href="https://dashboard.stackmob.com/schemas/" target="_blank">StackMob schemas list</a> that StackMob will have created your `todo` schema for you automatically. 

<p class="alert">
  If the schema doesn't already exist on the server, StackMob will auto create your schema when you make a `create` call. This only occurs in the development environment.
</p>

## Callback Functions: Success and Error

Notice that there are `success` and `error` callback functions above.  Most StackMob calls are done asynchronously via an AJAX call. This means that your code in the browser continues to run while StackMob continues to process your request.  `success` and `error` are guaranteed to only execute **after** the AJAX call has returned.

For example, let's try getting the JSON representation of an object after we've created it.

```js
var todo = new Todo({ ... });
todo.create();
todo.toJSON(); //this may not work because "create" may still be sending/processing the AJAX request!
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

To fetch your `todo` object, just specify the primary key and run `fetch`.

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

You can edit existing objects easily.  Let's update `todo` object `1234` with a new field: `finished` and let's mark it as `true`.

```javascript
var todo = new Todo({ todo_id: '1234' });
todo.save({ finished: true }, {
  success: function(model, result, options) { console.debug(model.toJSON()); }
  error: function(model, result, options) {}
});
```

<p class="alert">
  If you save a field that previously didn't exist, StackMob will add that to your schema automatically (only in the development environment).
</p>

You can also set fields and save things later.

```javascript
var todo = new Todo({ todo_id: '1234' });

todo.set({ finished: true });

todo.save({
  success: function(model, result, options) { console.debug(model.toJSON()); }
  error: function(model, result, options) {}
});
```

## Deleting Objects

Let's now delete objects.


```javascript
var todo = new Todo({ todo_id: '1234' });
user.destroy({
  success: function(model, result, options) { console.debug(model.toJSON()); },
  error: function(model, result, options) {}
});
```

That's it!


## Appending to an array

If you have arrays, you can append and delete from arrays pretty easily without having to update the whole object.  This helps prevent concurrency issues when people are appending to the same array/object.  Use `appendAndSave(fieldName, values, options)`

```javascript
<script type="text/javascript">
  var todo = new Todo({ todo_id: '12345' });
  todo.fetch();
  
  /*
    Resulting in:
   	{
   		todo_id: '12345',
  		title: 'Laundry',
  		subtasks: ['buy detergent', 'buy fabric softener', 'fold clothes'] //array of strings
  	}
   */
  
  .....
  
  todo.appendAndSave('subtasks', ['hang clothes', 'rinse and repeat']);
  
  ....
  
  /*
    Has this result saved on the server-side:
    {
   		todo_id: '12345',
  		title: 'Laundry',
  		subtasks: ['buy detergent', 'buy fabric softener', 'fold clothes', 'hang clothes', 'rinse and repeat'] 
  	}
   */
</script>
```

If you have multiple users appending to the same array concurrently, StackMob makes sure that each append is respected (and not overwritten as you would run into if you did a straight-up `save` on the object).  

## Deleting from an array

Now let's delete from an array without having to update the entire object.

```javascript
<script type="text/javascript">
  var todo = new Todo({ todo_id: '12345' });
  todo.fetch();
  
  /*
    Results in:
   	{
   		todo_id: '12345',
  		action: 'Do Laundry',
  		subtasks: ['buy detergent', 'buy fabric softener', 'fold clothes', 'hang clothes', 'rinse and repeat'] //array of strings
  	}
   */
  
  .....
  
  todo.deleteAndSave('subtasks', ['rinse and repeat']);
  
  ....
  
  /*
    Has this result saved on the server-side:
    {
   		todo_id: '12345',
  		action: 'Do Laundry',
  		subtasks:  ['buy detergent', 'buy fabric softener', 'fold clothes', 'hang clothes']
  	}
   */
</script>
```

As with `appendAndSave`, concurrency issues are handled appropriately.


# Object Collections

You'll likely be dealing with more than one object at a time though.  That's where `StackMob.Collection` comes in.  A `Collection` of objects gives you some convenient ways to manage and query for several objects at once.


<p class="alert">A <code>StackMob.Collection</code> is built upon Backbone.js's <code>Collection</code> and hence has all <a href="http://documentcloud.github.com/backbone/#Collection" target="_blank">methods of a Backbone.js Collection</a>.</p>

Let's define an object that will represent a **list** of your `Todo` objects.

```javascript

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

  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var Todos = StackMob.Collection.extend({
    model: Todo
  });

/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  ...
</script>

</body>
</html>

```

Extending from `StackMob.Collection` gives your list some important methods that help you query for multiple `Todo` items on StackMob.  Let's use that `Todos` collection to get all `Todo` objects saved on StackMob:

```javascript
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

  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var Todos = StackMob.Collection.extend({
    model: Todo
  });

/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  var todos = new Todos();
  todos.fetch({
    success: function(collection) {
      //after we've gotten all the Todo items from StackMob, let's see what we have!
      console.debug(todos.toJSON());
    }
  });
</script>

</body>
</html>
```

## Fetch Filtered Objects with StackMob.Collection.Query

You can request certain objects to be returned from StackMob by using `StackMob.Collection.Query`.  `StackMob.Collection.Query` gives you a way to specify what you're looking for.  Use it in conjunction with `StackMob.Collection`'s `query(..)` method.

```javascript
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

  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var Todos = StackMob.Collection.extend({
    model: Todo
  });

/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
  var smQuery = new StackMob.Collection.Query();
  //get all Todo items of high priority, order by createddate in ascending order.
  //Get the first 5 results.
  smQuery.equals('priority', 'high').orderAsc('createddate'); //you can chain query parameters
  smQuery.setRange(0,4); //or add them individually


  var todos = new Todos();
  //use StackMob.Collection's "query" method to get your results from StackMob
  todos.query(smQuery, {
    success: function(collection) {
      //print out the todos after the query returns from StackMob
      console.debug(todos.toJSON());
    }
  });
</script>

</body>
</html>
```


# Relationships

Relationships give you the ability to "join" objects together and return the data about the related objects in a single call.  In the below examples, were giving a `user` things to do by giving him a one-to-many relationship of `todo` items.

Let's assume you've <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships">added some relationships</a> to your object.  



## Adding Related Objects

You can create objects and add them to a related parent object in one swoop.

Let's assume you have a `User` with a one-to-many relationship with `Todo`, represented as a field called `chores`.  Let's assign some more chores for `Marty`. 

The `Todo` items will be created in the `todo` table *and* their IDs will be appended to the relationship array on `Marty`, updating the `Marty` user records in the `user` table - all in one call.

```javascript
<script type="text/javascript">
	var chore1 = new Todo({ action: 'Save Doc!' });
	var chore2 = new Todo({ action: 'Save Mom and Dad!' });

  var user = new StackMob.User({ username: 'Marty' });
  user.addRelationship('chores', [chore1, chore2], { ..success... });
  
  ...
  
  user.fetch();
  
  /*
   Results in:
   {
     username: 'Marty',
     chores: [ 
     		'ID for some existing todo that Marty already has', 
     		'ID for Save Doc!', 
     		'ID for Save Mom and Dad!' ], //We've appended ID for "Save Doc and ID for "Save Mom and Dad""     
     'createddate': ...,
     'lastmoddate': ...
   }
   
   Meanwhile, the instances of the Todo objects are saved in the "todo" table.
   */
  
</script>
```


## Fetching Relationships (Join)

The big advantage of defining relationships is that you can query for a lot of information in a single API call.  The data is generally the same as what you'd want a traditional SQL "join" to return.  You can query for a `user` and get full instances of the related `chores` he has in a single call using the `expand` feature.

```javascript
<script type="text/javascript">
  var user = new StackMob.User({ username: 'Marty' });
  
  //No "expand"
  user.fetch(); 
  
  /*
   Results in:
   {
     username: 'Marty',
     chores: [ '123', '456', 789'], 
     'createddate': ...,
     'lastmoddate': ...
   }
   */
  
  //Fetching WITH "expand"
  user.fetchExpanded(1);
  
  /*
   Results in:
   {
     username: 'Marty',
     chores: [
     	{
     		todo_id: '123',
     		action: 'Wash Biff's Car',
     		createddate: ...,
     		lastmoddate: ... 
     	}, 
     	{
     		todo_id: '456',
     		action: 'Save Doc!',
     		createddate: ...,
     		lastmoddate: ... 
     	},
			{
     		todo_id: '789',
     		action: 'Save Mom and Dad!',
     		createddate: ...,
     		lastmoddate: ... 
     	}
     ], 
     'createddate': ...,
     'lastmoddate': ...
   }
   */
  
</script>
```

Notice that with `fetchExpanded`, you don't have to query for the user and then make separate queries for the individual `Todo` items.  Instead, you can just fetch the expanded user!

If `Todo` had related objects, you can keep expanding deeper with `fetchExpanded(2)`.  StackMob lets you expand up to a **depth of 3**.  

## Deleting Related Objects

You can delete related objects in a single API call - without having to update the entire object.  This is also "thread safe" in that if multiple users delete related relationships from the object, StackMob will correctly handle each request and process them appropriately.

## Soft Delete

You can remove a relationship from an object (but keep the related object in the database) with a soft delete by passing the third parameter below as `false`.

```javascript
<script type="text/javascript">
  var user = new StackMob.User({ username: 'Marty' });
  user.deleteAndSave('chores', ['123', '456'], StackMob.SOFT_DELETE, { ..success... }); //delete the Todo objects with IDs of 123 and 456
  
  /*
   //Before delete:
   {
     username: 'Marty',
     chores: [ 
     		'123', '456', '789'     
     'createddate': ...,
     'lastmoddate': ...
   }
   
   //After delete
   {
     username: 'Marty',
     chores: [ 
     		'789'     
     'createddate': ...,
     'lastmoddate': ...
   }
   
   */
  
  var todos = new Todos();
  todos.fetch();
  //The above query will return all todo's, including those with ids 123, 456, and 789 because they're still in the database.
  
  
</script>
```

### Hard Delete

But you may want to delete a relationship **and** remove the actual related object from the database.  Pass in `true` to do a hard delete.

```javascript
<script type="text/javascript">
  var user = new StackMob.User({ username: 'Marty' });
  user.deleteAndSave('chores', ['123', '456'], StackMob.HARD_DELETE, { ..success... }); //delete the chores with IDs of 123 and 456
  
  /*
   //Before delete:
   {
     username: 'Marty',
     chores: [ 
     		'123', '456', '789'     
     'createddate': ...,
     'lastmoddate': ...
   }
   
   //After delete
   {
     username: 'Marty',
     chores: [ 
     		'789'     
     'createddate': ...,
     'lastmoddate': ...
   }
   
   */
  
  var todos = new Todos();
  todos.fetch();
  //The above query will return only [{ todo_id: '789' ... }] (only the 789 instance)
  
</script>
```


# User Authentication

StackMob also gives you a way to authenticate your users.  The JS SDK now uses OAuth 2.0 to login. It uses your `user` schema to perform login.

## Creating a User

StackMob provides a `StackMob.User` class for you out of the box.  Let's create a `StackMob.User` below, saving it to the server.

```js
var user = new StackMob.User({ username: 'Bill Watterson', password: 'weirdosfromanotherplanet', profession: 'cartoonist'  });
user.create({
  success: function(model, result, options) {},
  error: function(model, result, options) {}
});
```

That's it!  You've saved a new user to the server!

What's up with the dot in `StackMob.User`?  Don't worry.  It's not any magical javascript - we just wanted to make sure `User` didn't clash with any of your own variables, and so we namespaced `User` under `StackMob`.

<p class="alert"><code>username</code> is the default primary key for <code>StackMob.User</code> objects.  <code>password</code> is a special field that gets encrypted on the server.</p>

## Fetching a User

Fetching a user is simple.  Just create a `StackMob.User` object with the `username` of the user you want retrieved. (`username` is the default primary key of `StackMob.User` objects.) Then call `fetch`.


```javascript
var user = new StackMob.User({ username: 'Bill Watterson' });
//fetches the user "Bill Watterson" asynchronously
user.fetch({
  success: function(model) {
    //After StackMob returns "Bill Watterson", print out the result
    console.debug(model.get('username') + ': ' + model.get('profession'));
  }
});
```

## Declaring your own User Object type

Are you using a schema besides `user`?  Maybe you renamed your `username` and `password` fields during the schema creation process.  Declare a new User type.

```js
var Customer = StackMob.User.extend({
  
});
```

## Login/Logout

OAuth 2.0 login requires some basic setup.

### Login

For a step by step visit <a href="https://stackmob.com/devcenter/docs/Authenticating-Users-with-the-JS-SDK" target="_blank">Authenticating Users with OAuth 2.0: The JS SDK</a>.

But as a summary, you'll:

1. prepare a form for login 
2. prepare a destination page to handle the successful redirect the server will send back.  

And you'll have a logged in user.

### Logout

Logging out is simple too:

```javascript
<script type="text/javascript">
	//Let's logout a user
	user.logout();  
</script>
```

## Checking Login Status

You'll probably want to show different UI depending on whether a user is logged in or not.  

To check if *any* user is logged in, use `StackMob.isLoggedIn()`.

To check to see if a particular user is logged in, use `StackMob.isUserLoggedIn(username)` or use the user's method `user.isLoggedIn()`.

Let's login `chucknorris` below and try out the various methods to see what they would print.

```javascript
<script type="text/javascript">
//Let's start with no user logged in.

StackMob.isLoggedIn(); //evaluates to false
StackMob.isUserLoggedIn('chucknorris'); //evalutes to false

//Now let's login "chucknorris"
var user = new StackMob.User({ username: 'chucknorris', password: 'myfists' });
user.login();

...

//After he's logged in, let's call these methods again

StackMob.isLoggedIn(); //evaluates to true

StackMob.isUserLoggedIn('chucknorris'); //evalutes to true

StackMob.isUserLoggedIn('marty'); //evalutes to false

user.isLoggedIn(); //evaluates to true (because this user instance is 'chucknorris'

//Let's check if "Marty" is logged in.  Nope!
var invaliduser = new StackMob.User({ username: 'marty' });
invaliduser.isLoggedIn(); //evaluates to false (because user2 is 'marty');

</script>
```


# There's more to learn!

Check the <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" target="_blank">Javascript SDK API documentation page</a> for more!

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-forgotpassword" target="_blank">Forgot Password/Reset Password</a>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-createuserwithfacebook" target="_blank">Facebook Integration</a>
* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.geopoint" target="_blank">Geo Spatial Queries</a>

In the meantime, feel free to visit the Javascript API Docs for more info and examples. <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" class="button blue">Read StackMob Javascript API Docs</a>