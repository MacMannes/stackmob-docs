# Android: Parse to StackMob Tutorial

<a href="https://www.stackmob.com/parse/">&larr; Back to Migrate from Parse</a>

# Introduction

If you're an Android developer looking to switch from Parse, have no fear; we'll provide a quick comparison of features from their SDK to those found in the StackMob Java Client SDK. From the code examples, you'll see that making the switch to StackMob is really easy.

# Getting Started

You initialize the Parse SDK using both your public and private keys:

```java
public void onCreate() { 

	...

    Parse.initialize(this, "YOUR_PUBLIC_KEY", "YOUR_PRIVATE_KEY"); 

    ...
}
```

With StackMob, you initialize an instance of `StackMobAndroid`, using only your public key:

```java
public void onCreate() { 

	...

	StackMobAndroid.init(getApplicationContext(), 0, "9844010e-094f-4b5a-b784-530275cf452b");

 	...
}
```

In addition, StackMob allows you to specify an API version of your app. StackMob provides both development and production environments for your schemas. For more information, check out the <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">API Versions module in the StackMob Marketplace.</a>

# Creating objects

Creating an object in Parse requires the use of `ParseObject`:

```java

ParseObject gameScore = new ParseObject("GameScore");
gameScore.put("score", 1337);
gameScore.put("playerName", "Carl Atupem");
gameScore.put("cheatMode", false);
gameScore.saveInBackground();

```

With StackMob, you can extend the `StackMobModel` class for your objects:

```java

package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;
 
public class GameScore extends StackMobModel {

    private int score;
    private String playerName;
    private boolean cheatMode;
 
    public GameScore(int score, String playerName, boolean cheatMode) {
        super(GameScore.class);
        this.score = score;
        this.playerName = playerName;
        this.cheatMode = false;
    }
}

```
then create:

```java

GameScore gameScore = new GameScore(1337, "Carl Atupem", false);
gameScore.save(new StackMobModelCallback() {
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

# Querying objects

The Parse SDK uses the `ParseQuery` class to search for objects:

```java

ParseQuery query = new ParseQuery("GameScore");
query.whereEqualTo("playerName", "Carl Atupem");
query.findInBackground(new FindCallback() {
    public void done(List<ParseObject> scoreList, ParseException e) {
        if (e == null) {
            Log.d("score", "Retrieved " + scoreList.size() + " scores");
        } else {
            Log.d("score", "Error: " + e.getMessage());
        }
    }
});

```

Here, StackMob's approach is similar; you use the StackMobQuery class to accomplish the same thing:

```java
GameScore.query(GameScore.class, new StackMobQuery().field(new StackMobQueryField("playerName")
.isEqualTo("Carl Atupem")), new StackMobQueryCallback<GameScore>() {
    @Override
    public void success(List<GameScore> result) {
       // You've now got a list game scores
    }
 
    @Override
    public void failure(StackMobException e) {
    }
});
```

# Users

Parse uses `ParseUser` to create User objects:

```java

ParseUser user = new ParseUser();
user.setUsername("my name");
user.setPassword("my pass");
user.setEmail("email@example.com");
 
// other fields can be set just like with ParseObject
user.put("phone", "650-253-0000");
 
user.signUpInBackground(new SignUpCallback() {
  public void done(ParseException e) {
    if (e == null) {
      // Hooray! Let them use the app now.
    } else {
      // Sign up didn't succeed. Look at the ParseException
      // to figure out what went wrong
    }
  }
});

```

With StackMob, you can extend the `StackMobUser` class, which is a specialized subclass of StackMobModel:

```java

package com.stackmob.snapstack;

import com.stackmob.sdk.model.StackMobUser;

public class User extends StackMobUser {
	
	String email;
	String phone;

	public User(String username, String password, String email, String phone) {
		super(User.class, username, password);
	}

}

```

Then you can call save:

```java

User user = new User("Carl Atupem", "STACKMOBRULEZ123", "carl_atupem@aol.com", "617-666-1234");
user.login(new StackMobModelCallback() {
 
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
}

```

## Facebook Users

Here's how Parse's Facebook login looks:

```java

ParseFacebookUtils.initialize("YOUR FACEBOOK APP ID");

...

ParseFacebookUtils.logIn(this, new LogInCallback() {
  @Override
  public void done(ParseUser user, ParseException err) {
    if (user == null) {
      Log.d("MyApp", "Uh oh. The user cancelled the Facebook login.");
    } else if (user.isNew()) {
      Log.d("MyApp", "User signed up and logged in through Facebook!");
    } else {
      Log.d("MyApp", "User logged in through Facebook!");
    }
  }
});

```

Here is the same Facebook login with StackMob:

```java

// Passing true creates a user if needed
user.loginWithFacebook(facebookToken, true, "Carl Atupem", new StackMobOptions(), new StackMobModelCallback() {
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
});

```

The StackMob Facebook login method allows you to specify whether or not to create Users if they don't already exist. For more information, <a href="http://developer.stackmob.com/tutorials/android/Working-with-Facebook" target="_blank">check out our Facebook integration tutorial.</a>

## Twitter Users

Here's what Parse's Twitter login looks like:

```java

ParseTwitterUtils.initialize("YOUR CONSUMER KEY", "YOUR CONSUMER SECRET");

ParseTwitterUtils.logIn(this, new LogInCallback() {
  @Override
  public void done(ParseUser user, ParseException err) {
    if (user == null) {
      Log.d("MyApp", "Uh oh. The user cancelled the Twitter login.");
    } else if (user.isNew()) {
      Log.d("MyApp", "User signed up and logged in through Twitter!");
    } else {
      Log.d("MyApp", "User logged in through Twitter!");
    }
  }
});

```

Here's that same Twitter login looks like, for StackMob:

```java

// Passing true creates a user if needed
user.loginWithTwitter(twitterToken, twitterSecret, true, "@atupem", new StackMobOptions(), new StackMobModelCallback() {
    @Override
    public void success() {
    }
 
    @Override
    public void failure(StackMobException e) {
    }
});

```

StackMob again provides the option to create Users automatically if they don't exist. For more information, <a href="http://developer.stackmob.com/tutorials/android/Working-with-Twitter" target="_blank">check out our Twitter integration tutorial.</a>

# GeoPoints

GeoPoints look like this in Parse:

```java

ParseGeoPoint point = new ParseGeoPoint(40.0, -30.0);

```

With StackMob, your code is virtually the same, using the StackMobGeoPoint class:

```java

StackMobGeoPoint point = new StackMobGeoPoint(40.0, -30.0);

```

# Files

Parse handles storage of files using `ParseFile`, and requires you to save the file before attaching it to an object:

```java

byte[] data = "Working at Parse is great!".getBytes();
ParseFile file = new ParseFile("resume.txt", data);
file.saveInBackground();

ParseObject jobApplication = new ParseObject("JobApplication");
jobApplication.put("applicantName", "Joe Smith");
jobApplication.put("applicantResumeFile", file);
jobApplication.saveInBackground();


```

StackMob's powerful S3 integration allows for binary data to be attached to objects and saved to S3 seamlessly.

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
<a href="http://developer.stackmob.com/tutorials/android/Upload-to-S3" target="_blank">Check out our S3 tutorial for more information.</a>

# Push Notifications

Here's how sending a push message looks with Parse:

```java

ParsePush push = new ParsePush();
push.setChannel("Giants");
push.setMessage("The Giants just scored! It's now 2-2 against the Mets.");
push.sendInBackground();

```

StackMob's push messaging works similarly: 

```java

Map<String, String> payload = new HashMap<String, String>(); 
payload.put("payload", getPushPayload()); 
user.sendPush(payload, standardToastCallback);

```

<a href="http://developer.stackmob.com/tutorials/android/Push-Notifications" target="_blank">Check out our Push tutorial for more information.</a>

# Custom Code

With Parse's "Cloud Code", developers write endpoints in JavaScript, and call them from the client like so:

```java

ParseCloud.callFunction("hello", new HashMap<Object, Object>(), new FunctionCallback<String>() {
   void done(String result, ParseException e) {
       if (e == null) {
          // result is "Hello world!"
       }
   }
});

```

StackMob offers Custom Code, which allows developers to write powerful methods in either Java or Scala. Making a Custom Code request is simple:

```java

StackMob.getStackMob().getDatastore().get("hello_world", new StackMobCallback() {
    @Override public void success(String responseBody) {
        //responseBody is "{ \"msg\": \"Hello, world!\" }"
    }
    @Override public void failure(StackMobException e) {
    }
});

```


# What Are You Waiting For?

There you have it, our overview is complete. Throughout this guide we showcased the similarities of the StackMob and Parse SDKs, and where necessary, highlighted the advantages of StackMob. So what are you waiting for? <a href="https://dashboard.stackmob.com/signup" target="_blank" class="btn btn-warning btn-rounded">Sign Up</a> and start building your mobile services on the StackMob platform.
