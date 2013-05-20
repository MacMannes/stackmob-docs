Developer Guide
=====================================

# Setup

## Initializing the Android SDK

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

## Defining a Model

```java,1,3
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

# Datastore

## Create an Save Object

```java
Task myTask = new Task("Learn more about StackMob", new Date());
myTask.save();
```

## Asynchronous Calls

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

## Read an Object


[HOW TO SET ID?]

```java
Task myTask = new Task("1234");
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

## Update an Object

```java
Task myTask = new Task("1234");
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

## Delete an Object

```java
Task myTask = new Task("1234");
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

## Arrays


```java
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

## Deleting from an Array

code for deleting from array

# Queries

## Comparison

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

## Relationship and Arrays

(in queries)

## Pagination

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

## Ordering

## Select Fields

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

## Chaining

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

# Relationships

You can have relationships between data objects so that they can be easily loaded together. You can do this simply by having models as child objects. Here are some examples, starting with our standard `Task` class

## One to One Relationship

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

## One to Many Relationship

Let's now relate one object to many children objects.

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

## Adding Related Objects

create a related object and associate it with the parent in one API call

## Decoupling Related Objects

de associating a related object from the parent

# User Authentication

## Creating a User

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

## Fetching a User

## Custom User Model

how do you declare a custom user modeL?

with custom username/password fields?

## Login

```java
User user = new User("AzureDiamond", "hunter2");
user.login(new StackMobModelCallback() {
 
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
}
```

## Logout

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

## Checking Login Status

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


## Change Password

```java
user.resetPassword("[old password]", "[new password]", new StackMobModelCallback() {
 
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
}
```

## Password Recovery

```java
StackMobUser.sendForgotPasswordEmail("johndoe", new StackMobModelCallback() {
    @Override public void success() {
    }
 
    @Override public void failure(StackMobException e) {
    }
});
```

# Access Controls

# Files

StackMob lets you save large files to S3 as easily as saving any other data to StackMob's cloud. The files are then accessible via an S3 url. To start, make sure you've [configured your app for S3](https://developer.stackmob.com/tutorials/android/Upload-to-S3). For this example we've got a binary field named photo on the schema `task` and a corresponding field on our `Task` object of type `StackMobFile`.

```java
package com.example.yourapp;
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

# Geolocation

To use geopoints, you first need to create a geopoint field in your schema. Go to [Manage Schemas](), edit your schema, and create a field of type Geopoint.

![Add a Geopoint field](http://static.stackmob.com/images/android/geopoint.png)

For this example we've got a Geopoint field named location on the schema `task` and a corresponding field on our `Task` object of type `StackMobGeoPoint`.

```java
package com.example.yourapp;
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

# Social Integration

## Facebook

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

## Twitter

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

# Push Notifications

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

<h3>Unregister for push</h3>
When a user unsubscribes from push messages you'll want to remove their tokens from StackMob

```java
user.removeFromPush(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```

<h3>Get a user's tokens</h3>
If you do want to see the raw tokens for a user you can do so:

```java
user.getPushTokens(new StackMobPushToken(getRegistrationID()), standardToastCallback)
```
The callback will be invoked with a json array that can be serialized to `StackMobPushToken`


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

<h3>Broadcast push message</h3>

Sending a push message to all registered tokens for your app is called a broadcast

```java
Map<String, String> payload = new HashMap<String, String>();
payload.put("payload", getPushPayload());
StackMobPush.getPush().broadcastPushNotification(payload, standardToastCallback);
```

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

# Deploy

