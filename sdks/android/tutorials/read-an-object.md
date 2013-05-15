Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Reload an existing object from the server.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

* <a href="https://developer.stackmob.com/tutorials/android/Save-an-Object">Create an object to reload</a>

<h1>Let's get started</h1>


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

and we saved one called `myTask` to StackMob. Suppose we think that object has changed on the server; we want to reload our local object to get the latest. You can simulate this by going to the <a href="https://dashboard.stackmob.com/data/browser/task">object browser</a> and changing values for the object you just saved. Then in your app simply call

```java
myTask.fetch(new StackMobModelCallback() {
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

In the success callback you can inspect the `myTask` object in the debugger or print values to verify that it has in fact been changed. Note that this isn't threadsave, so your app should take care not to alter objects while they're in the process of being saved.

<h2>Build and Run!</h2>

Run your project. 

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)