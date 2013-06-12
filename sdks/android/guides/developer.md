Developer Guide
=====================================

StackMob's Android SDK enables your application to take advantage of StackMob's REST API on Android devices.  It's your app's interface to StackMob's services.

## Overview

The StackMob Android SDK is Model based, meaning that if you declare a class that represents an object, you can directly save and fetch from that object and StackMob will make appropriate save and fetch calls to the datastore for you, populating your object asynchronously.

The Android SDK is built off of the StackMob Java SDK, so a lot of this content is applicable for both the Android SDK and the Java SDK.  You'll see references to them in this guide, but for the most part they overlap seamlessly.

In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p>

## Setup

### Initializing the SDK

The Android SDK is built on top of the Java SDK, so we've provided the initialization of both SDKs here.  Simply choose your SDK.

In both SDKs you'll need <a href="https://dashboard.stackmob.com/settings" target="_blank">your development public key on your app dashboard</a>.

<p class="screenshot"><a href="https://dashboard.stackmob.com/settings" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/setup/app-settings-keys.png" alt="Your App Keys"></a></p>

Initialize the SDK with your public key.

<span class="tab setup" title="Android SDK"></span>

<p><b>Android SDK</b></p>

```java,6,7
public class MainActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        StackMobAndroid.init(getApplicationContext(), 0, 
        	"YOUR PUBLIC KEY");
    }
}
```

Need to download and setup the SDK? 

<a href="https://developer.stackmob.com/android-sdk/configure">Setup and Configure the Android SDK <i class="icon-chevron-right"></i></a>

<span class="tab"></span>

<span class="tab setup" title="Java SDK"></span>

<p><b>Java SDK</b></p>

The Java SDK runs in OAuth 1.0 mode or OAuth 2.0 mode.  Read <a href="" target="_blank">how StackMob uses OAuth 1.0 and OAuth 2.0</a> to decide which one you would like to use.

<p><b>Initialize the SDK for OAuth 2.0</b></p>

```java
import com.stackmob.sdk.api.StackMob
Integer apiVersion = 0;

StackMob client = new StackMob(apiVersion, "YOUR PUBLIC KEY");
```

<p><b>Initialize the SDK for OAuth 1.0</b></p>

```java
import com.stackmob.sdk.api.StackMob
Integer apiVersion = 0;

// set the api secret to null if you use oauth version 2
StackMob client = new StackMob(StackMob.OAuthVersion.ONE, 
    apiVersion, "YOUR PUBLIC KEY", "YOUR PRIVATE KEY"); 
```

<a href="https://developer.stackmob.com/java-sdk/configure">Setup and Configure the Java SDK <i class="icon-chevron-right"></i></a>

<span class="tab"></span>

### Defining a Model

Let's say you're creating a simple todo-list app, where you have an object type called `Task`.  Even before we get to the backend, datastore, and all that good stuff, let's define our class.

Define your `Task` class by extending from `StackMobModel`.  Doing this will give your object its StackMob save/fetch abilities.  We'll also add two fields: `name` and `dueDate`.

```java,1,3
import com.stackmob.sdk.model.StackMobModel;
 
public class Task extends StackMobModel {
 
    private String name;
    private Date dueDate;
 
    public Task(String name, Date dueDate) {
        super(Task.class);
        this.name = name;
        this.dueDate = dueDate;
    }

    public void setName(String name) { this.name = name; }
    public String getName() { return this.name; }

    public void setDueDate(Date dueDate) { this.dueDate = dueDate; }
    public Date getDueDate() { return this.dueDate; }
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
      </ul>
    </div>
  </div>
</div>

## Datastore

Let's use the Android SDK to persist objects to the datastore and retrieve them.  **You don't have to setup any databases beforehand!**

### Create a Save Object

Save an instance of your `task` object to the server.

```java
Task myTask = new Task("Learn more about StackMob", new Date());
myTask.save();
```

That's it!  That fires off an HTTP request to StackMob's REST API which saves your object.  StackMob will **automatically create your schema** for you if it doesn't already exist.  By extending `StackMobModel`, we'll automatically serialize your fields into JSON and save your data against a new `task` schema, which we've inferred from your classname.

Here's the data on the server, as seen through the Object Browser:

<p class="screenshot"><a href="https://dashboard.stackmob.com/data/browser/task" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/objectbrowser-task-basic.png" alt="You first Task object in the Object Browser"></a></p>

StackMob created a schema for you on the fly by inferring it from the JSON sent to the API.  Here's the brand new `task` schema being listed in the Schemas UI.

<p class="screenshot"><a href="" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/schema-list-task.png" alt="List of schemas with your new Task schema"></a></p>

And here're the fields.

<p class="screenshot"><a href="https://dashboard.stackmob.com/schemas/edit/task" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/schema-task-basic-fields.png" alt="List of Task fields"/></a></p>

StackMob automatically generates some fields for you on the server-side too.

* `task_id` (the primary key) - primary keys will have the format of `[schemaName]_id` and are auto generated by StackMob
* `createddate` - the created date in milliseconds
* `lastmoddate` - the last modified date in milliseconds
* `sm_owner` - who created the object (we'll get into this later!)

<p class="alert">
  If the schema doesn't already exist on the server, StackMob will auto create your schema when you make a <code>create</code> call. (This only occurs in the development environment.)
</p>

If you'd like to set your own ID rather than having StackMob generate it for you, just set the ID first.

```java
Task task = new Task("Learn how to set the primary key", new Date());
task.setID("1");
task.save();
```

This will assign the primary key as `1`.  If the key already exists, the call will fail.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html#save()" target="_blank">StackMobModel#save</a></li>
      </ul>
    </div>
  </div>
</div>

### Asynchronous Calls

Most StackMob calls are done asynchronously. This means that your code on the client continues to run while your HTTP request to StackMob may still be en route to the server.  `success` and `failure` are guaranteed to only execute **after** the call has returned, so if you have some action that you want to run after a `save` call, be sure to put it in the `success` or `failure` callbacks.

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/callback/StackMobCallback.html" target="_blank">StackMobCallback</a></li>
      </ul>
    </div>
  </div>
</div>

### Read an Object

To fetch your `task` object, just specify the primary key and run `fetch`.  Again, the primary key takes the form of `[schemaName]_id`.  Below, we'll assume the primary key is "1234".

```java
Task myTask = new Task();
myTask.setID("1234");
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

The above makes an HTTP REST API call to StackMob.  A JSON object is returned, and we automatically fill your `myTask` object.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html#fetch(com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobModel#fetch</a></li>
      </ul>
    </div>
  </div>
</div>

### Update an Object

You can edit existing objects easily.  Let's update `task` object `1234` with a new field: `done` and let's mark it as `true`.

```java
Task myTask = new Task();
myTask.setID("1234");
myTask.setIsDone(true);
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

(We assume above that we've added a new `done` field.)

```java,5,11,14,15
public class Task extends StackMobModel {
 
    private String name;
    private Date dueDate;
    private boolean done;
 
    public Task(String name, Date dueDate) {
        super(Task.class);
        this.name = name;
        this.dueDate = dueDate;
        this.done = false;
    }

    public boolean getIsDone() { return this.done; }
    public void setIsDone(boolean isDone) { this.done = isDone; }
}
```


<p class="alert">
  If you save a <b>field</b> that previously didn't exist, StackMob will add that to your schema automatically (only in the development environment).
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html#save()" target="_blank">StackMobModel#save</a></li>
      </ul>
    </div>
  </div>
</div>

### Delete an Object

Let's assume you have created an object Task and saved it to StackMob.
Say we store it in a variable called `myTask`.
Now you can delete the object by calling `myTask.destroy()`

```java
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

You can also delete an object by its ID. Let's delete Task object with ID `1234`

```java
Task myTask = new Task();
myTask.setID("1234"); // set the object ID here
myTask.destroy();
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html#destroy()" target="_blank">StackMobModel#destroy</a></li>
      </ul>
    </div>
  </div>
</div>



### Create Multiple Objects at Once

Wnat to create multiple objects in one call?

```java
Task firstTask = new Task(...);
Task secondTask = new Task(...);
Task.saveMultiple(Arrays.asList(firstTask, secondTask), new StackMobCallback(...));
```

This saves multiple objects in a single request call.

StackMob currently do not support multiple updates at once.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobModel.html" target="_blank">StackMobModel</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html#saveMultiple(java.util.List, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobModel#saveMultiple</a></li>
      </ul>
    </div>
  </div>
</div>



### Arrays

You can save arrays.  Let's add to our `Task` class so that it has an array of notes (`strings`) you can jot down.

```java,4,9,17-19,21-23
public class Task extends StackMobModel {
    private String name;
    private Date dueDate;
    private List<String> notes;
 
    public Task(String name, Date dueDate) {
        super(Task.class);
        this.dueDate = dueDate;
        this.notes = new ArrayList<String>();
        this.name = name;
    }
 
    public String getName() {
        return name;
    }
 
    public List<String> getNotes() {
        return notes;
    }

    public List<String> setNotes(List<String> notes) {
        this.notes = notes;
    }
}
```

You can save arrays as you normally would:

```java
List<String> notes = new ArrayList<String>();
notes.add("check the Android Developer Guide");
notes.add("I was last reading about arrays");

Task myTask = new Task("Learn more Android!", new Date());
myTask.setNotes(notes)
myTask.save(...);
```

An array of strings will be saved on the server side.


## Queries

To query objects by field parameters, paginate, sort by, and more, use the static `query(..)` method provided by `StackMobModel`.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobQuery.html" target="_blank">StackMobQuery</a></li>
      </ul>
    </div>
  </div>
</div>

### Comparison

Let's get `Task` objects where a `priority` integer field is greater than `3`.  The success callback will produce a list of matching `Task` objects.

```java
// Assume we have `priority` field in our schema
// Which means that we have an `int priority` field in our `Task` class
Task.query(Task.class, new StackMobQuery().fieldIsGreaterThan("priority", 3), new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority tasks
       // You can iterate over them here to generate some UI.
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobQuery.html" target="_blank">StackMobQuery</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html#fieldIsEqualTo(java.lang.String, int)" target="_blank">StackMobQuery#fieldIs...</a></li>
      </ul>
    </div>
  </div>
</div>




### Chaining

You can chain together query parameters.  Let's search for `Task` objects with `priority > 3` and `done=true`.

```java
Task.query(Task.class, 
        new StackMobQuery().fieldIsGreaterThan("priority", 3).
            field(new StackMobQueryField("done").isEqualTo(true)), 
        new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority, completed tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

### Querying for Multiple Values

Looking for matching elements in arrays?  Let's get objects that have `A` or `B` in the `notes` array.  e.g. a `Task` object with `{ notes: ['A', 'C', 'E'], ...}` would be returned because it matched on `A`.

```java
Task.query(Task.class, new StackMobQuery().fieldIsIn("notes", Arrays.asList("A", "B")), 
        new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority, completed tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobQuery.html" target="_blank">StackMobQuery</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html#fieldIsIn(java.lang.String, java.util.List)" target="_blank">StackMobQuery#fieldIsIn</a></li>
      </ul>
    </div>
  </div>
</div>


### Pagination

You can paginate over results.  Let's grab the first ten.

```java
Todo.query(Todo.class, new StackMobQuery().isInRange(0, 9), 
        new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of all tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobQuery.html" target="_blank">StackMobQuery</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html#isInRange(java.lang.Integer, java.lang.Integer)" target="_blank">StackMobQuery#isInRange</a></li>
      </ul>
    </div>
  </div>
</div>


### Select Fields

Only return back a subset of fields, reducing the payload.  Get the first ten `Task` objects but only return the `name` and `priority` fields.  (IDs will be returned also, as StackMob always returns the ID).  This will reduce the amount of data sent over the network, if bandwidth is a concern.


```java
    Task.query(Task.class, new StackMobQuery().isInRange(0, 9), 
            StackMobOptions.selectedFields(Arrays.asList("name", "priority")), 
            new StackMobQueryCallback<Task>() {
        @Override
        public void success(List<Task> result) {
           // You've now got a list of the tasks
        }
 
        @Override
        public void failure(StackMobException e) {
        }
    });
```

This will return JSON objects with just three fields:

* todo_id
* name
* priority

Again, the ID is returned by default no matter what fields you select.

```js
[
  {
    todo_id: '1234',
    name: 'Learn more Android!',
    priority: 3
  },
  {
    todo_id: '4567',
    name: 'Learn even more Android!',
    priority: 2
  },
  ...
]
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobOptions.html" target="_blank">StackMobOptions</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobOptions.html#selectedFields(java.util.List)" target="_blank">StackMobOptions#selectedFields</a></li>
      </ul>
    </div>
  </div>
</div>


### Chaining

```java
Task.query(Task.class, new StackMobQuery().fieldIsGreaterThan("priority", 3).
        field(new StackMobQueryField("done").isEqualTo(true)), 
        new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
       // You've now got a list of high priority, completed tasks
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

### count

Need to check the number of objects there are in the datastore?  Use `count`.

Here let's see how many tasks have `priority > 3` and `done=true`.

```java
Task.query(Task.class, new StackMobQuery().fieldIsGreaterThan("priority", 3).
        field(new StackMobQueryField("done").isEqualTo(true)), 
        new StackMobCountCallback() {
    @Override
    public void success(int count) {
       //number of results
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/index.html?com/stackmob/sdk/api/StackMobQuery.html" target="_blank">StackMobQuery</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/callback/StackMobCountCallback.html" target="_blank">StackMobCountCallback</a></li>
      </ul>
    </div>
  </div>
</div>

## Relationships

StackMob supports relationships between objects.  For example a `user` can be associated with many `task` objects.  StackMob saves one-to-many relationships as an array of IDs rather than an array of JSON objects.  Here's a JSON representation of `john`, a user, with three related `task` objects.

```js
//user object
{
  username: 'john',
  tasks: ['123', '234', '345'] //example of todo IDs
}
```

Where `123`, `234`, `345` are example IDs of todo objects.  Saving by ID makes editing a child object much easier - you only have to edit one instance of the object, even if it's saved on various parent objects.  (Versus if we saved embedded JSON objects - then copies of the objects would be everywhere.)

Defining relationships lets you fetch and save information more easily.  You can:

* create related objects and attach them to a parent object in one call
* fetch a parent object and have the full JSON objects returned as well, not just the ids.

    ```js
    {
      username: 'john',
      chores: [
         { task_id: '123', action: '...', ...}, 
         { task_id: '234', action: '...', ...}, 
         { task_id: '345', action: '...', ...}]
    }
    ```

<p class="alert">
  StackMob handles relationships by reference.  StackMob does not embed JSON within JSON.  There's only one instance of it in the datastore, making your data more easily editable and flexible.
</p>


### Relating Objects

Relationships are stored as arrays, so you can handle relationships with simple array operations.

```java
//show how you can save task ids to the array and do "save"

```


Let's get to it.




### Creating Relationships with the Dashboard

Let's give a `user` object several `todo` items.  We'll assume that we have both `todo` and `user` schemas already defined.

<a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">Edit the user schema</a> and add a Relationship to the user.

<p class="screenshot"><a href="" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/relationships/dashboard-schemas-relationships-add.png" alt=""/></a></p>

Fill in relationship details:

<p class="screenshot"><a href="" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/relationships/dashboard-schemas-relationships-add-task-modal.png" alt=""/></a></p>

And you'll get:

<p class="screenshot"><a href="" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/relationships/dashboard-schemas-relationships-task-field.png" alt=""/></a></p>

**Save the schema** and that's it - you have a relationship.

Let's work with relationships in the code.


### One to One Relationship

```java
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

Each `Task` object now can have a link to another `Task` object. This is called a one-to-one relation. Any child of a class extending `StackMobModel`, or any collection of them, becomes a relation like this and is stored independently. Now let's take a look at a new object.

### One to Many Relationship

Let's now relate one object to many children objects.

```java
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

## Users

StackMob gives you a way to authenticate your users.  The Android SDK uses OAuth 2.0 to login. It uses your `user` schema to perform login.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobUser.html" target="_blank">StackMobUser</a></li>
      </ul>
    </div>
  </div>
</div>


### Creating a User

Let's create a user object.  Below, our `User` class extends `StackMobUser` (which in turn extends `StackMobModel`).  `StackMobUser` will provide us authentication methods.

```java
import com.stackmob.sdk.model.StackMobUser;
 
public class User extends StackMobUser {
 
    private List<TaskList> taskLists;
    private String email;
 
    protected User(String username, String password) {
        super(User.class, username, password);
    }
 
 
    public List<TaskList> getTaskLists() {
        return taskLists;
    }
 
    public void setTasks(List<TaskList> taskLists) {
        this.taskLists = taskLists;
    }
}
```

Create an instance

```java
User user = new User("bob", "mypassword");
user.save();
```

<p class="alert"><code>username</code> is the default primary key for <code>StackMob.User</code> objects.  <code>password</code> is a special field that gets encrypted on the server.</p>

### Fetching a User

Fetching a user is simple.  Like other objects, just provide the primary key.

```java
User user = new User("bob");
user.fetch(...);
```

### Login

Let's login your user.  If you recall, we have a `User` object with a `username/password` constructor.  Let's login that user.

```java
User user = new User("bob", "mypassword");
user.login(new StackMobModelCallback() {
 
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
}
```

Your user will be logged in for an hour.

* Logging in from another device will invalidate the session on other devices.

<p class="alert">
  StackMob's uses OAuth 2.0 authentication, the industry standard.  When logging in, users are issued an accessToken that will be used to sign their requests to identify them.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobUser.html" target="_blank">StackMobUser</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#login(java.util.Map, com.stackmob.sdk.api.StackMobOptions, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#login</a></li>
      </ul>
    </div>
  </div>
</div>

### Logout

Logging out is easy.  Simply call `logout` on your user object.

```java
user.logout(new StackMobModelCallback() {
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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobUser.html" target="_blank">StackMobUser</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#logout(com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#logout</a></li>
      </ul>
    </div>
  </div>
</div>

### Checking Login Status

Check to see if a user is logged in.  These methods are asynchronous because they also check the server in some instances.

```java
if(StackMob.getStackMob().isLoggedIn()) {
    User.getLoggedInUser(User.class, new StackMobQueryCallback<User>() {
        @Override
        public void success(List<User> list) {
            User loggedInUser = list.get(0);
            continueWithApp(loggedInUser);
        }
 
        @Override
        public void failure(StackMobException e) {
            User newlyLoggedInUser = doLogin();
            continueWithApp(newlyLoggedInUser);
        }
    });
} else {
    User newlyLoggedInUser = doLogin();
    continueWithApp(newlyLoggedInUser);
}
```


<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMob.html#isLoggedIn()" target="_blank">StackMob#isLoggedIn,isUserLoggedIn,isLoggedOut</a>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#isLoggedIn()" target="_blank">StackMobUser#isLoggedIn</a></li>
      </ul>
    </div>
  </div>
</div>



### Change Password

If your user is logged in, your user may want to change his/her password.  To do so, you can call `resetPassword` on their behalf.  They'll have to provide their current password and their new password.

```java
user.resetPassword("[current password]", "[new password]", new StackMobModelCallback() {
 
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobUser.html" target="_blank">StackMobUser</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#resetPassword(java.lang.String, java.lang.String, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#resetPassword</a></li>
      </ul>
    </div>
  </div>
</div>


### Password Recovery

If your user has forgotten his or her password, they can request that an email with a temporary password be sent to an email address.  That email address should be a field in the user schema.  You'll need to specify which field represents the email address when defining your User schema.

[Screenshot of Forgot password UI in user schema]

Call `sendForgotPasswordEmail` and specify the `username` (primary key) of the user you want to send the email to.

```java
StackMobUser.sendForgotPasswordEmail("bob", new StackMobModelCallback() {
    @Override public void success() {
    }
 
    @Override public void failure(StackMobException e) {
    }
});
```

<p class="alert">Be sure to send it to the <code>username</code>, not the email address!</p>


<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/?com/stackmob/sdk/model/StackMobUser.html" target="_blank">StackMobUser</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#sentForgotPasswordEmail(java.lang.String, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#sentForgotPasswordEmail</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#loginResettingTemporaryPassword(java.lang.String, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#loginResettingTemporaryPassword</a></li>
      </ul>
    </div>
  </div>
</div>



## Access Controls


StackMob provides you the ability to lock down access to your data with Access Controls.  You can disallow access to Create, Read, Update or Delete.  With user authentication, you can even restrict permissions at a user level.

Here, we set it so that only logged in users can create objects for this schema.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/Scrumptious/dashboard-acl.png" alt=""/>

So now if you call `tasks.fetch(..)`, you'll get a 401 Unauthorized error if you aren't logged in.  Because the logic is handled server-side, nobody can mess with your control logic even if they view the client side code.

<a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">Read about all the Access Control options.</a> Find which one works best for your use case.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>References</strong>
      <ul>
        <li><a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">Access Control Options</a></li>
      </ul>
    </div>
  </div>
</div>

## File Storage

StackMob lets you save large files to S3 as easily as saving any other data to StackMob's cloud. The files are then accessible via an S3 url. To start, make sure you've [configured your app for S3](https://developer.stackmob.com/tutorials/android/Upload-to-S3). For this example we've got a binary field named photo on the schema `task` and a corresponding field on our `Task` object of type `StackMobFile`.

```java
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private StackMobFile photo;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}


	public void setPhoto(StackMobFile photo) {
		this.photo = photo;
	}

	public StackMobFile getPhoto() {
		return photo;
	}
}
```

If you set a `StackMobFile` to `photo` and then save your object, 

```java
byte[] image = // ... get the bytes of whatever you're uploading
Task myTask = new Task("Upload some files", new Date());
myTask.setPhoto(new StackMobFile("image/jpeg", "mypicture.jpg", image);
test.save(new StackMobModelCallback() {
    @Override
    public void success() {
        System.out.println(test.getPhoto().getS3Url());
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```
Once the success callback is called, you can check `getS3Url()` to get the url the photo has been uploaded to. Once you've successfully uploaded your file the local data will be cleared to save memory. To delete the object from StackMob and the file from S3, simply call

```java
myTask.destroy()
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobFile.html" target="_blank">StackMobFile</a></li>
      </ul>
    </div>
  </div>
</div>


## Geolocation

To use geopoints, you first need to create a geopoint field in your schema. Go to [Manage Schemas](), edit your schema, and create a field of type Geopoint.

![Add a Geopoint field](http://static.stackmob.com/images/android/geopoint.png)

For this example we've got a Geopoint field named location on the schema `task` and a corresponding field on our `Task` object of type `StackMobGeoPoint`.

```java
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private StackMobGeoPoint location;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}


	public void setLocation(StackMobGeoPoint location) {
		this.location = location;
	}

	public StackMobGeoPoint getLocation() {
		return location;
	}
}
```

[StackMobGeoPoint](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobGeoPoint.html) is a longitude/latitude pair representing a point. It uses radians, and you can convert distances to radians via the static methods on `StackMobGeoPoint`. You can now save and query for your `Task` object, and the location field will be treated like any other data. Once you have some objects with geopoints stored you use them for spacially
aware queries. 

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobGeoPoint.html" target="_blank">StackMobGeoPoint</a></li>
      </ul>
    </div>
  </div>
</div>

<h3>Near Queries</h3>

Near queries allow you to get a list of objects sorted by distance from a given `GeoPoint`. You can optionally specify a maximum distance to search. Any ordering you specify will be ignored. Here's a query to find all `Task` objects
within 10 miles of San Francisco.

```java
StackMobGeoPoint sf = new StackMobGeoPoint(122.68, 37.73);
StackMobQuery q = new StackMobQuery().fieldIsNearWithinMi("location", sf, 10); //(lon, lat), distance (miles)
Task.query(Task.class, q, new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
		for(Task task : result) {
			System.out.println(task.getLocation().getQueryDistanceRadians());
		}
    }

    @Override
    public void failure(StackMobException e) {
    }
});

```
Note how the `StackMobGeoPoint` objects you get back from a near query have a property `getQueryDistanceRadians` available. This is the distance between that point and the reference (in this case San Francisco), which can be handy.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html#fieldIsNear(java.lang.String, com.stackmob.sdk.api.StackMobGeoPoint)" target="_blank">StackMobQuery#fieldIsNear (radians, mi, km)</a></li>
      </ul>
    </div>
  </div>
</div>


<h3>Within Queries</h3>
Within queries search for results within a specified radius of a given point or within a 2-dimensional box. These queries do not have a natural ordering. Unlike near queries, if you specify an ordering for the result set it will be honored. 

```java
StackMobGeoPoint sf = new StackMobGeoPoint(122.68, 37.73);
StackMobQuery q = new StackMobQuery().fieldIsWithinRadiusInMi("location", sf, 10); //(lon, lat), distance (miles)
Task.query(Task.class, q, new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
    }

    @Override
    public void failure(StackMobException e) {
    }
});

```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html#fieldIsWithinRadiusInKm(java.lang.String, com.stackmob.sdk.api.StackMobGeoPoint, java.lang.Double)" target="_blank">StackMobQuery#fieldIsWithin...</a></li>
      </ul>
    </div>
  </div>
</div>

## Social Integration

### Facebook

StackMob allows you to use Facebook for user authentication in place of or alongside the built-in [User Authentication](https://developer.stackmob.com/tutorials/android/User-Authentication). This means rather than having a username and password login screen, you can have a "login with facebook" button. 

Facebook has an SDK that allows you get get back a "Facebook Token" for a user, which can then be used as credentials with StackMob:

```java
User newUser = new User()
newUser.createWithFacebook(facebookToken, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

Once created and thereafter, you can use the Facebook token to log that user in:

```java
newUser.loginWithFacebook(facebookToken, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```
Alternatively, you can use the login method to create users. The method has a boolean in its parameters; when set to true, a user will be created if none exists. The user will be created with the username provided:

```java
// Passing true creates a user if needed
newUser.loginWithFacebook(facebookToken, true, "[username]", new StackMobOptions(), new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

If you have an existing user, created with a username/password or Twitter, you can also link it with a Facebook account to allow facebook login.

```java
existingUser.linkWithFacebook(facebookToken, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});

```

Being logged in with Facebook is exactly the same as being logged in regularly, with extra bits of functionality. StackMobUser has two methods only usable when logged in with facebook: `postFacebookMessage` and `getFacebookUserInfo`. These allow more interation with the actual Facebook account.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#loginWithFacebook(java.lang.String, boolean, java.lang.String, com.stackmob.sdk.api.StackMobOptions, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#loginWithFacebook</a></li>
      </ul>
    </div>
  </div>
</div>

### Twitter

StackMob allows you to use Twitter for user authentication in place of or allongside the built-in [User Authentication](https://developer.stackmob.com/tutorials/android/User-Authentication). This means rather than having a username and password login screen, you can have a "login with twitter" button. 

Twitter has an SDK that allows you get get back a "Twitter Token" and "Twitter Secret" for a user, which can then be used as credentials with StackMob:

```java
User newUser = new User()
newUser.createWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

Once created and thereafter, you can use the Twitter token and secret to log that user in

```java
newUser.loginWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

Alternatively, you can use the login method to create users. The method has a boolean in its parameters; when set to true, a user will be created if none exists. The user will be created with the username provided:

```java
// Passing true creates a user if needed
newUser.loginWithTwitter(twitterToken, twitterSecret, true, "[username]", new StackMobOptions(), new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

If you have an existing user, created with a username/password or Facebook, you can also link it with a Twitter account to allow twitter login.

```java
existingUser.linkWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});

```

Being logged in with Twitter is exactly the same as being logged in regularly, with extra bits of functionality. StackMobUser has two methods only usable when logged in with twitter: `postTwitterMessage` and `getTwitterUserInfo`. These allow more interation with the actual Twitter account.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#loginWithTwitter(java.lang.String, java.lang.String, boolean, java.lang.String, com.stackmob.sdk.api.StackMobOptions, com.stackmob.sdk.callback.StackMobCallback)" target="_blank">StackMobUser#loginWithTwitter</a></li>
      </ul>
    </div>
  </div>
</div>

## Push Notifications

<h3>Introduction to Android Push Notifications</h3>

StackMob integrates with Google's GCM service for push notifications on Android. Before you start using push notifications for Android on StackMob, you should familiarize yourself with the client-side aspects of GCM by reading the [Google Cloud Messaging Framework](http://developer.android.com/guide/google/gcm/gs.html) guide. Don't worry about the server-side aspects of push - that's what we're here for.

<h3>Android Push Notifications on StackMob</h3>

StackMob helps you with your Push Notification process by providing you the backend to send push messages. Follow these steps to get Push running for your Android application.

1. Create a Google API project, enable GCM, and generate your API Key following the directions at [http://developer.android.com/guide/google/gcm/gs.html](http://developer.android.com/guide/google/gcm/gs.html)
1. Note the "sender id" and "API Key" you generate. These will be used later
1. [Upload](https://dashboard.stackmob.com/module/push/settings/android) your API Key to StackMob.
1. Get a valid Android registration ID for your device for testing.
1. Send push notifications to your Android device from StackMob's web console.

<h3>Generating and Uploading your API Key</h3>

To use the push service you'll need to obtain a API Key from Google and [upload it to the StackMob server](https://dashboard.stackmob.com/module/push/settings/android). Upload your API Key to Sandbox. When you deploy your app to Production, you'll want to upload an API Key there as well. It can be the same one or a different one.

<h3>Sending and Receiving Push Messages</h3>

Now that you have an API Key, let's test sending & receiving push notifications. First, make sure yourn API Key is correctly saved [here](https://dashboard.stackmob.com/module/push/settings/android). Next, set up an Android app. You'll do the appropriate setup and write the code to create & register a registration ID and to receive push messages.


<h3>App Setup</h3>

If you're using the <a href="https://github.com/stackmob/stackmob-android-examples">Android Starter Project</a>, GCM Push is already set up for you. Just set up your [StackMob credentials](https://dashboard.stackmob.com/sdks/android/config, and add your sender id to the top of <a href="https://github.com/stackmob/stackmob-android-examples/blob/master/PushDemo/src/com/stackmob/pushdemo/PushDemoActivity.java" target="_blank">`PushDemoActivity.java`</a>

```java
	public static String SENDER_ID = "YOUR_SENDER_ID_HERE";
```

and you're ready to go. You can grow your app for the starter, or rip it apart for the bits you want

You can also set up your app manually, using [Google's instructions](http://developer.android.com/guide/google/gcm/gs.html). The StackMob sdk already contains gcm.jar

Note, that StackMob Push isn't set up for OAuth2 yet, so you'll have to add the OAuth1 initialization.

```java
	StackMobAndroid.init(this.getApplicationContext(), StackMob.OAuthVersion.One, 0, "YOUR_API_KEY_HERE", "YOUR_API_SECRET_HERE");
```

The demo push project is already set up for this, so you won't need to make any changes there.

<h3>Interacting with StackMob</h3>

In the `onRegistered` callback of `GCMIntentService` you get the registration id, which is the last piece you need to start talking to stackmob. Check out the [Push Docs](http://stackmob.com/devcenter/docs/Push-API) for examples of how to use StackMob to register your device and start pushing.

<h3>PushTokens</h3>
`StackMobPushToken` represents the information needed to identify a device for push. You can construct them with the "registration id" you get back from registering your device with GCM. By default it assume GCM, but you can also work with C2DM or iOS tokens if you need to.

<h3>Registering for push</h3>
StackMob keeps a collection of push tokens for your app, and you must add a new token to the collection before you can send messages to that device. You can associate the token with a user, which allows you to think in terms of users rather than tokens. A user can have multiple tokens on different devices, and the tokens can be expired and replaced by GCM at any time on the server, so this is good practice. All tokens on the server will be used when you do a broadcast message.

```java
User user = new User("<username>", "<optional password>");
user.registerForPush(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#registerForPush(com.stackmob.sdk.push.StackMobPushToken, com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobUser#registerForPuash</a></li>
      </ul>
    </div>
  </div>
</div>

<h3>Unregister for push</h3>
When a user unsubscribes from push messages you'll want to remove their tokens from StackMob

```java
user.removeFromPush(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#removeFromPush(com.stackmob.sdk.push.StackMobPushToken, com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobUser#removeFromPush</a></li>
      </ul>
    </div>
  </div>
</div>

<h3>Get a user's tokens</h3>
If you do want to see the raw tokens for a user you can do so:

```java
user.getPushTokens(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```
The callback will be invoked with a json array that can be serialized to `StackMobPushToken`

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#getPushTokens(com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobUser#getPushTokens</a></li>
      </ul>
    </div>
  </div>
</div>


<h3>Pushing to a user</h3>
To actually send a push to a specific user, create a map with your information, and make this call.
```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
user.sendPush(payload, standardToastCallback);
```

You can send a message to multiple users at once also

```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
User.pushToMultiple(payload, Arrays.asList(user1, user2, user3), standardToastCallback);
```

It's simple to save your `User` objects, query for them to get various groupings, and then send push messages.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#sendPush(java.util.Map, com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobUser#sendPush</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html#pushToMultiple(java.util.Map, java.util.List, com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobUser#pushToMultiple</a></li>
      </ul>
    </div>
  </div>
</div>

<h3>Broadcast push message</h3>

Sending a push message to all registered tokens for your app is called a broadcast

```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
StackMobPush.getPush().broadcastPushNotification(payload, standardToastCallback);
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html" target="_blank">StackMobPush</a></li>
        <li><a href="http://stackmob.github.io/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html#broadcastPushNotification(java.util.Map, com.stackmob.sdk.callback.StackMobRawCallback)" target="_blank">StackMobPush#broadcastPushNotification</a></li>
      </ul>
    </div>
  </div>
</div>

<h3>Push without users</h3>

If you just want to do push without worrying about users (if you're only ever doing broadcasts for example) you can find a lower level API in [StackMobPush](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html). You can user an arbitrary string for username if you don't plan on using it at all

* Read the javadocs for [StackMobUser](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html) and [StackMobPush](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/push/StackMobPush.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)

<h3>Push in Development vs. Production</h3>

Once you set your version to 1 and use the production keys, StackMob will use Apple's production push server 
instead of the Apple sandbox.

You should only use the dev keys with version 0 and the production keys with version 1.

The user tokens don't change, so you would still test in the same manner.

Here is some additional info on <a href="http://developer.stackmob.com/tutorials/dashboard/API-Versioning">API versioning with development and production environments</a>.

## Deploy

You're done developing your app!  You've been working in StackMob development environment and you now want to get everything to the production environment.  StackMob let's you do that easily.  Let's go through what you'll need to do.

### API

StackMob gives you separate development and production environments so that you can keep your test data and custom code separate from your production set.  Deploying your API is a *critical* step you need to do when deploying.  StackMob provides a simple UI to help you do this easily.

Read about how to <a href="https://developer.stackmob.com/deploy" target="_blank">Deploy your API</a>

<p class="screenshot"><a href="https://developer.stackmob.com/tutorials/dashboard/Deploying-your-StackMob-App" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/apiversions/modules-apiversions-deploy.png" alt=""/></a></p>

After deploying, you would point your JS SDK at your production API Version - in this case `1`.  Also, use the <a href="https://dashboard.stackmob.com/settings" target="_blank"><b>production</b> public key</a> we provide you:

```js,3
StackMob.init({
  publicKey: "YOUR PRODUCTION PUBLIC KEY",
  apiVersion: 1
});
```

That's it!  With your server rolled out to API Version 1, you can point your JS SDK at your production server and start making calls.  Your production environment has a different database than your development one, so you'll be able to continue developing in API Version 0 without affecting your production customers.

<p class="alert">
  You can have <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">multiple production API Versions</a> so that you can deploy multiple clients pointing at older versions of your API.  StackMob's <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">Multiple API Versions module</a> makes backwards compatibility easy.
</p>
