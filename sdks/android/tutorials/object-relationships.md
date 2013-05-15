Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Learn to save and load child objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~10 minutes

<h3>Prerequisites</h3>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

<h1>Let's get started!</h1>

<h2>Open the StackMob Starter App</h2>

In previous tutorials you've seen how you can extend `StackMobModel` to get a data model that can be saved and loaded from the server. However, beyond just storing data, you probably want to have relationships between data objects, so that they can be easily loaded together. You can do this simply by having models as child objects. Here are some examples, starting with our standard `Task` class

```java
package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private Task prerequisite;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}

	public void setPrerequisite(Task prereq) {
		this.prerequisite = prereq;
	}
}
```

Each `Task` object now can have a link to another `Task` object. This is called a one-to-one relation. Any child of a class extending `StackMobMob`, or any collection of them, becomes a relation like this and is stored independently. Now let's take a look at a new object.

```java
package com.stackmob.taskmob;

import java.util.ArrayList;
import java.util.List;

import com.stackmob.sdk.model.StackMobModel;

public class TaskList extends StackMobModel {

	private String name;
	private List<Task> tasks;
	private Task topTask;
	private TaskList parentList;

	public TaskList(String name) {
		super(TaskList.class);
		tasks = new ArrayList<Task>();
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public List<Task> getTasks() {
		return tasks;
	}
}
```

This class represents a collection of tasks with a name and some other bells and whistles. The `topTask` and `parentList` fields are both one-to-one relations like before, while `tasks`, being a list, is a one-to-many relation.

These relations make it easy to save and load entire trees of data. To do, simple specify `StackMobOptions.depthOf(x)` when doing a `save`, `fetch`, or `query`. 

```java
TaskList taskList = new TaskList("StackMob Tasks");
taskList.getTasks().add(new Task("Learn about relations", new Date()));
taskList.getTasks().add(new Task("Learn about queries", new Date()));
taskList.save(StackMobOptions.depthOf(1));
```

You can specify depths of up to 3. Take a look at [TaskList](https://dashboard.stackmob.com/data/browser/tasklist) and [Task](https://dashboard.stackmob.com/data/browser/task) in the object browser; you should see the objects you've just saved.


Here's a query that will return all `TaskList` objects and their child objects.

```java
   TaskList.query(TaskList.class, new StackMobQuery(), StackMobOptions.depthOf(1), new StackMobQueryCallback<TaskList>() {
       @Override
       public void success(List<TaskList> result) {
	       
       }

       @Override
       public void failure(StackMobException e) {
       }
   });
```

And here's reloading a whole tree:

```java
taskList.fetch(StackMobOptions.depthOf(1), new StackMobModelCallback() {
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

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)