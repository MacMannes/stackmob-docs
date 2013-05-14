# Add Related Objects

# Overview

## Objectives

Add related objects to an object

## Experience Level

Beginner

## Estimated Time to Complete

~5 minutes

## Prerequisites

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships#a-adding_a_one_to_many_relationship" target="_blank">Define a relationship in your schema</a>

* Create a user (either using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create tutorial</a>)

**Let's get started!**

# Related API

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-save" target="_blank">save</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-appendandsave" target="_blank">appendAndSave</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-addrelationship" target="_blank">addRelationship</a>

Relationships give you the ability to "join" objects together and return the data about the related objects in a single call. In the below examples, we are giving a `User` things to do by giving him a one-to-many relationship of `Todo` items.

## Add a related object

Let's assume you have a `User` with a one-to-many relationship with `Todo`, represented as a field called `chores`. We'll assign some more chores for user Marty.

The `Todo` items will be created in the `Todo` table and their IDs will be appended to the relationship array on Marty, updating the Marty user records in the `User` table.

Use `appendAndSave` for adding relationships to existing objects.

```js
  // Declare this once on the page to associate a schema
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });
     
  var chore1 = new Todo({ action: "Save Doc!" });
     
  // Get an existing user
  var user = new StackMob.User({ username: "Marty" });

  // Save a chore
  chore1.save(null, {
    success: function(){

      // Then assign it to a user
      user.appendAndSave('chores', [chore1.get('todo_id')] );
    },
    error: function(){ console.log("There was an error saving the chore") }
  })

```

And the relationship is added to the user:

```js
  user.fetch();
  /*
  Results in:
  {
    username: 'Marty',
    chores: [ 
           'ID for some existing todo that Marty already has', 
           'ID for Save Doc!'], //We've appended the ID for the new chore
    'createddate': ...,
    'lastmoddate': ...
  }

  */
    
```

Alternatively, you can save and add relationships all in one call with `addRelationship`.

Use `addRelationship` for adding relationships to new objects.

```js
  var chore2 = new Todo({ action: "Save Mom and Dad!" });
  var chore3 = new Todo({ action: "Wash Biff's Car" });
  
  user.addRelationship('chores', [chore2, chore3]);
  // Instances of the Todo objects are saved to the "todo" table.
  // Also, the relationships for these two objects are saved to the user table at the same time.
```


What about one-to-one relationships?  Just pass one object instead of an array.  Let assume that `chore` is a one-to-one relationship.

```js
  var chore1 = new Todo({ action: 'Save Doc!' });
  
  var user = new StackMob.User({ username: 'Marty' });
  user.addRelationship('chore', chore1);
```