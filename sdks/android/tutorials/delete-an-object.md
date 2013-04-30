Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)


<h3>Objective</h3>

Delete an existing object.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://dashboard.stackmob.com/sdks/android/config">set up for StackMob</a>

* <a href="https://developer.stackmob.com/tutorials/android/Save-an-Object">Create an object to delete</a>

<h2>Let's get started</h2>

In the <a href="https://developer.stackmob.com/tutorials/android/Save-an-Object">previous tutorial</a> we had a `Task` object:

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

and we saved one called `myTask` to StackMob. Suppose we now want to delete it. 

```java
myTask.destroy();
```

It's good practice to set a callback on the `destroy` so you can verify that it got sent.

```java
Task myTask = new Task("Learn even more about StackMob", new Date());
myTask.destroy(new StackMobModelCallback() {
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

<h3>Build and Run!</h3>
Run your project. Go back to the <a href="https://dashboard.stackmob.com/data/browser/task" target="_blank">StackMob Object Browser</a> and click the Query button to refresh your list. You’ll notice the object has been deleted.

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)