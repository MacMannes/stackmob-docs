Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Reduce your network load by only querying for specific fields.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

* <a href="https://developer.stackmob.com/tutorials/android/Query-an-Object">Query for objects</a>

<h1>Let's get started!</h1>


When querying for objects, it's sometimes useful to only query for certain fields. Recall the `Task` object we've been using.

```java
package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private List<String> hugeIrrelevantList;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}
}
```

It now has a field `hugeIrrelevantList`. We want to get a list of the `Task` objects we've saved, but loading all the data in each object would slow us down needlessly. Let's grab the first 10 object, loading only the `name` and `priority` fields:

```java
    Task.query(Task.class, new StackMobQuery().isInRange(0, 9), StackMobOptions.selectedFields(Arrays.asList("name", "priority")), new StackMobQueryCallback<Task>() {
        @Override
        public void success(List<Task> result) {
 	       // You've now got a list of the tasks
        }
 
        @Override
        public void failure(StackMobException e) {
        }
    });
```

You now have a list of `Task` objects with only the `name` and `priority` fields filled in. All other fields are left to their defaults, `null` or 0, or anything they're initialized to in a zero-argument constructor. You can fill any other fields you want later by calling `fetch` on a particular object with `StackMobOptions.selectedFields`.

```java
	myTask.fetch(StackMobOptions.selectedFields(Arrays.asList("hugeIrrelevantList")), new StackMobModelCallback() {
        @Override
        public void success() {
			//myTask now has data for hugeIrrelevantList
        }
 
        @Override
        public void failure(StackMobException e) {
        }
    });
```

This feature works well with the expand functionality. Use the expand setting to set the depth of the object expansion, and then use the select feature to pick which fields are returned. You will get an error, if you try to select a field in an object, which you did not expand.

To access subobjects use the format `referenceField.referredField`, where `referenceField` is the field in the object that contains the reference, and `referredField` is the name of the field that you want to select. Suppose you have a schema with a user object, which has a field called `interest_ref`, which refers to an object called `interest`, and you want to select only username and the field `interest_type` in the `interest` object. 


Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html) and [StackMobQuery javadocs](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)