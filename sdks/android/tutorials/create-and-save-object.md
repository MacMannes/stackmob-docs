Want to just see it in action? Check out [Task.java](https://github.com/stackmob/stackmob-android-examples/blob/master/TaskMob/src/com/stackmob/taskmob/Task.java) and [TaskActivity.java](https://github.com/stackmob/stackmob-android-examples/blob/master/TaskMob/src/com/stackmob/taskmob/TaskActivity.java) in the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Create and save a new object.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

<h1>Let's get started!</h1>

For this example we've got an object representing a task in a TODO app. Create this class in a file called `Task.java

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

To save a task, simply create one and call save:

```java
Task myTask = new Task("Learn more about StackMob", new Date());
myTask.save();
```

It's good practice to set a callback on the save so you can verify that it got sent.

```java
Task myTask = new Task("Learn even more about StackMob", new Date());
myTask.save(new StackMobModelCallback() {
    @Override
    public void success() {
		// the call succeeded
    }

    @Override
    public void failure(StackMobException e) {
		// the call failed
    }
});
```

That's all you need to do. All details of how the object is stored are handled automatically!

<h2>Build and Run!</h2>

Run your project. You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/task" target="_blank">StackMob Object Browser</a>.

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)