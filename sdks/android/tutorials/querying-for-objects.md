Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Query for objects.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://dashboard.stackmob.com/sdks/android/config">set up for StackMob</a>

* <a href="https://developer.stackmob.com/tutorials/android/Save-an-Object">Create an object to reload</a>

<h1>Let's get started!</h1>


Now that you've got some objects stored on StackMob, how do you get access to them? Recall the `Task` object we've been using.

```java
package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}
}
```

Let's say you've saved a few of these already, and you want get load the first ten.

```java
Task.query(Task.class, new StackMobQuery().isInRange(0, 9), new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of all tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

More interestingly, you can add constraints to your query using `StackMobQuery`. Here's an example that returns all tasks with a `priority` greater than 3:

```java
Task.query(Task.class, new StackMobQuery().fieldIsGreaterThan("priority", 3), new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

And here's one for completed tasks:

```java
Task.query(Task.class, new StackMobQuery().field(new StackMobQueryField("done").isEqualTo(true)), new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

These can be chained together, so to get all high priority, completed tasks you would do

```java
Task.query(Task.class, new StackMobQuery().fieldIsGreaterThan("priority", 3).field(new StackMobQueryField("done").isEqualTo(true)), new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority, completed tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```


There's a wide array of constraints you can use here, checkout the [StackMobQuery javadocs](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html) for the full list.

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html) and [StackMobQuery javadocs](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)