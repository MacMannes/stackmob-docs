<h3>Objectives</h3>
Use increment and decrement on save on one of the `todo` objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/module/customcode" target="_blank">The custom code module installed</a>

* There is a `Todo` object which `todo_id` is `todo1` (create using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a>)

* `Todo` object has field `num_likes` as an integer

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="http://stackmob.github.com/stackmob-customcode-sdk/0.5.2/apidocs/com/stackmob/sdkapi/SMIncrement.html" target="_blank">SMIncrement</a>

<h2>Increment num_likes Field</h2>

Let's assume we have a social `Todo` list app where people have an option to like and unlike any `Todo` list.

Hence, we want to keep track how many likes we have gotten so far by incrementing our `num_likes` field as user clicks on like button.

```java
 	// get the StackMob datastore service and assemble the query
	DataService dataService = serviceProvider.getDataService();

    try {
        List<SMUpdate> update = new ArrayList<SMUpdate>();
        update.add(new SMIncrement("num_likes", 2));
        SMObject incrementResult = dataService.updateObject("todo", "todo1", update);

    } catch (Exception e) {
    	//log your error
  	}
```

<h2>Decrement num_likes Field</h2>

Now, we want to deal with the scenario of user clicking the unlike button. Hence, we are going to use decrement on save.

```java
// get the StackMob datastore service and assemble the query
DataService dataService = serviceProvider.getDataService();

try {
    List<SMUpdate> update = new ArrayList<SMUpdate>();
    update.add(new SMIncrement("num_likes", -2));
    SMObject incrementResult = dataService.updateObject("todo", "todo1", update);

} catch (Exception e) {
   	//log your error
}
```

Note: these increment and decrement on save methods are useful in some game apps, say you want to update a character's health point when it takes damage or uses health potion.