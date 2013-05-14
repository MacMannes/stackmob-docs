<h3>Objectives</h3>
Deleting related objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships#a-adding_a_one_to_many_relationship" target="_blank">Define a relationship in your schema</a>

* Add a related object to an object (<a href="https://developer.stackmob.com/tutorials/js/Add-Relationships" target="_blank">how to add related object</a>)

* Create a user

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-deleteandsave" target="_blank">deleteAndSave</a>

<h2>Delete Related Objects</h2>

You can delete related objects in a single API call - without having to update the entire object. This is also "thread safe" in that if multiple users delete related relationships from the object, StackMob will correctly handle each request and process them appropriately.

Let's assume you have a user `Marty` that has three `Todo` objects related to his user object through a relationship named `chores`.

<h3>Soft Delete</h3>

You can remove a relationship from an object (but keep the related object in the database) with a soft delete by passing the third parameter below as false.

```js
  /*
  
   //Before delete:
   {
     username: 'Marty',
     chores: [ 
            '123', '456', '789'     
     'createddate': ...,
     'lastmoddate': ...
   }
   
  */
  

  var user = new StackMob.User({ username: 'Marty' });
  // Soft delete the todos with ids 123 and 456 from `Marty`
  user.deleteAndSave('chores', ['123', '456'], StackMob.SOFT_DELETE, { ..success... });
   
  /*
    
   // After delete
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
  // This query will return all todo's, including those with ids 123, 456, and 789 because they're still in the database.
   
```

<h3>Hard Delete</h3>

Sometimes you may want to delete a relationship and remove the actual related object from the database. Pass in true to do a hard delete.

Let's assume you have a user `Marty` that has three `todo` objects related to his user object through a relationship named `chores`.

```js
  /*

   // Before delete:
   {
     username: 'Marty',
     chores: [ 
            '123', '456', '789'     
     'createddate': ...,
     'lastmoddate': ...
   }

  */

  // Hard delete the todos. This will still remove the items from `Marty` but will also delete the todo items with IDs 123 and 456 from the `Todo` table.
  var user = new StackMob.User({ username: 'Marty' });
  user.deleteAndSave('chores', ['123', '456'], StackMob.HARD_DELETE, { ..success... });
   
  /*
  
   // After delete
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
  // This query will return only [{ todo_id: '789' ... }] (only the 789 instance)
```