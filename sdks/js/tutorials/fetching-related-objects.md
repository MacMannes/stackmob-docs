<h3>Objectives</h3>
Fetch related objects

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

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-fetch" target="_blank">fetch</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-fetchexpanded" target="_blank">fetchExpanded</a>

<h2>Fetch Related Objects</h2>

The big advantage of defining relationships is that you can query for a lot of information in a single API call. The data is generally the same as what you'd want a traditional SQL "join" to return. You can query for a user and get full instances of the related `chores` he has in a single call using the `expand` feature.

```js
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
   
```

Or use `fetchExpanded` to fetch the related objects instead of just their ids.

```js
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
```

Notice that with `fetchExpanded`, you don't have to query for the user and then make separate queries for the individual `Todo` items. Instead, you can just fetch the expanded user!

If `Todo` had related objects, you can keep expanding deeper with `fetchExpanded(2)`. StackMob lets you expand up to a **maximum depth of 3**.