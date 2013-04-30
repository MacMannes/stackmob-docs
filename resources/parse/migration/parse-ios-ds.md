# iOS: Parse to StackMob Tutorial

<a href="https://www.stackmob.com/parse/">&larr; Back to Migrate from Parse</a>

# Introduction

If you're an iOS developer looking to switch from Parse, have no fear; we'll provide a quick comparison of features from their SDK to those found in the StackMob iOS SDK. We'll also highlight some of the advantages StackMob presents. From the code examples, you'll see that making the switch to StackMob is really easy.

The StackMob iOS SDK utilizes powerful Core Data integration. For more information, check out our <a href="http://stackmob.github.io/stackmob-ios-sdk/">iOS SDK Reference</a>. If you're new to Core Data, <a href="http://go.stackmob.com/learncoredata.html">check out our Core Data ebook.</a>

However, we understand that if you're not familiar with Core Data, but more with Parse's syntax, you may not feel comfortable yet making such a big leap.  But don't worry, the code examples in this guide will refer to **our iOS SDK's Lower Level DataStore**, to provide familarity to those used to using Parse.  When you're ready, we strongly encourage you to take advantage of the features we provide with Core Data including offline caching and more.


 

# Getting Started

You initialize the Parse SDK using both your public and private keys:

```obj-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

	[Parse setApplicationId:@YOUR_PUBLIC_KEY clientKey:@YOUR_PRIVATE_KEY];

	...

}
```

With StackMob, you initialize an instance of `SMClient`, using only your public key:

```obj-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

	// Declare a variable in your AppDelegate.h file called SMClient *client;
	self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];

	...
}
```
In addition, StackMob allows you to specify an API version of your app. StackMob provides both development and production environments for your schemas. For more information, check out the <a href="https://marketplace.stackmob.com/module/apiversions" target="_blank">API Versions module in the StackMob Marketplace.</a>

# Creating objects

Creating an object in Parse requires the use of `PFObject`, an extension of `NSDictionary`:

```obj-c

PFObject *gameScore = [PFObject objectWithClassName:@"GameScore"];
[gameScore setObject:[NSNumber numberWithInt:1337] forKey:@"score"];
[gameScore setObject:@"Sean Plott" forKey:@"playerName"];
[gameScore setObject:[NSNumber numberWithBool:NO] forKey:@"cheatMode"];
[gameScore saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
  if (!error) {
    // The gameScore saved successfully.
  } else {
    // There was an error saving the gameScore.
  }
}];
```

With StackMob, you simply use `NSDictionary` to create objects, and pass them to `SMDataStore` for creation:

```obj-c

NSMutableDictionary *gameScore = [NSMutableDictionary dictionary];
[gameScore setObject:[NSNumber numberWithInt:1337] forKey:@"score"];
[gameScore setObject:@"Sean Plott" forKey:@"playerName"];
[gameScore setObject:[NSNumber numberWithBool:NO] forKey:@"cheatMode"];

SMDataStore *dataStore = [[SMClient defaultClient] dataStore];
[dataStore createObject:gameScore inSchema:@"GameScore" onSuccess:^(NSDictionary *object, NSString *schema) {
    // The gameScore saved successfully.
} onFailure:^(NSError *error, NSDictionary *object, NSString *schema) {
    // There was an error saving the gameScore.
}];
```

# Querying objects

The Parse SDK uses `PFQuery` to search for objects:

```obj-c

PFQuery *query = [PFQuery queryWithClassName:@"GameScore"];
[query whereKey:@"playerName" equalTo:@"Carl Atupem"];
[query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
  if (!error) {
    // The find succeeded.
    NSLog(@"Successfully retrieved %d scores.", objects.count);
  } else {
    // Log details of the failure
    NSLog(@"Error: %@ %@", error, [error userInfo]);
  }
}];

```

Here, StackMob's approach is similar; you use SMQuery to achive the same thing:

```obj-c

SMQuery *query = [[SMQuery alloc] initWithSchema:@"GameScore"];
[query where:@"playerName" isEqualTo:@"Carl Atupem"];

SMDataStore *dataStore = [[SMClient defaultClient] dataStore];
[dataStore performQuery:query onSuccess:^(NSArray *results) {

    // The find succeeded.
    NSLog(@"Successfully retrieved %i scores.", results.count);
} onFailure:^(NSError *error) {

    // Log details of the failure
    NSLog(@"Error: %@", error); 
}];
```

# Users

Parse uses `PFUser` to create User objects:

```obj-c

PFUser *user = [PFUser user];
user.username = @"my name";
user.password = @"my pass";
user.email = @"email@example.com";

// other fields can be set just like with PFObject
[user setObject:@"415-392-0202" forKey:@"phone"];

[user signUpInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
  if (!error) {
      // Hooray! Let them use the app now.
  } else {
      NSString *errorString = [[error userInfo] objectForKey:@"error"];
      // Show the errorString somewhere and let the user try again.
  }
}];

```

With StackMob, you can create users the same way you would regular objects:

```obj-c

NSMutableDictionary *user = [NSMutableDictionary dictionary];
[user setObject:@"Carl Atupem" forKey:@"username"];
[user setObject:@"STACKMOBRULEZ123" forKey:@"password"];
[user setObject:@"carl_atupem@aol.com" forKey:@"email"];
[user setObject:@"617-666-1234" forKey:@"phone"];

SMDataStore *dataStore = [[SMClient defaultClient] dataStore];
[dataStore createObject:user inSchema:@"User" onSuccess:^(NSDictionary *object, NSString *schema) {
    // The user object saved successfully.
} onFailure:^(NSError *error, NSDictionary *object, NSString *schema) {
    // There was an error saving the user object.
}];
```

## Facebook Users

Here's how Parse's Facebook login looks:

```obj-c

[PFFacebookUtils initializeFacebook];

...

[PFFacebookUtils logInWithPermissions:permissions block:^(PFUser *user, NSError *error) {
    if (!user) {
        NSLog(@"Uh oh. The user cancelled the Facebook login.");
    } else if (user.isNew) {
        NSLog(@"User signed up and logged in through Facebook!");
    } else {
        NSLog(@"User logged in through Facebook!");
    }
}];
```

Here is the same Facebook login with StackMob:

```obj-c

[[SMClient defaultClient] loginWithFacebookToken:@"FACEBOOK_TOKEN" createUserIfNeeded:YES 
	usernameForCreate:@"Carl Atupem" onSuccess:^(NSDictionary *result) {
    	NSLog(@"User logged in through Facebook!");
} onFailure:^(NSError *error) {
    	NSLog(@"Error logging in through Facebook: %@", error);
}];

```

The StackMob Facebook login method allows you to specify whether or not to create Users if they don't already exist. For more information, <a href="http://developer.stackmob.com/tutorials/ios/Integrating-with-Facebook" target="_blank"></a>check out our Facebook integration tutorial.

## Twitter Users

Here's what Parse's Twitter login looks like:

```obj-c
[PFTwitterUtils initializeWithConsumerKey:@"YOUR CONSUMER KEY"
                           consumerSecret:@"YOUR CONSUMER SECRET"];

...

[PFTwitterUtils logInWithBlock:^(PFUser *user, NSError *error) {
    if (!user) {
        NSLog(@"Uh oh. The user cancelled the Twitter login.");
        return;
    } else if (user.isNew) {
        NSLog(@"User signed up and logged in with Twitter!");
    } else {
        NSLog(@"User logged in with Twitter!");
    }     
}];
```

Here's that same Twitter login looks like, for StackMob:

```obj-c
[[SMClient defaultClient] loginWithTwitterToken:@"TWITTER_TOKEN" twitterSecret:@"TWITTER_SECRET" 
	createUserIfNeeded:YES usernameForCreate:@"@atupem" onSuccess:^(NSDictionary *result) {
        NSLog(@"User logged in with Twitter!");
} onFailure:^(NSError *error) {
       NSLog(@"Error logging in through Twitter: %@", error); 
}];
```
StackMob again provides the option to create Users automatically if they don't exist. For more information, <a href="http://developer.stackmob.com/tutorials/ios/Integrating-with-Twitter" target="_blank">check out our Twitter integration tutorial.</a>


# GeoPoints

GeoPoints look like this in Parse:

```obj-c
PFGeoPoint *point = [PFGeoPoint geoPointWithLatitude:40.0 longitude:-30.0];
```

With StackMob, your code is virtually the same, using the SMGeoPoint class:

```obj-c
SMGeoPoint *point = [SMGeoPoint geoPointWithLatitude:40.0 longitude:-30.0];
```

Also provided is a helper method with a built-in CLLocationManager, which makes it hassle free to grab a user's current location:

```obj-c
[SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
     NSLog(@"Current location: %@", geoPoint);   
} onFailure:^(NSError *error) {
     NSLog(@"Error getting SMGeoPoint: %@", error);  
}];
```

# Storage

Parse handles storage of files using `PFFile`, and requires you to save the file before attaching it to an object:

```obj-c
NSData *imageData = UIImagePNGRepresentation(image);
PFFile *imageFile = [PFFile fileWithName:@"image.png" data:imageData];
[imageFile save];

PFObject *userPhoto = [PFObject objectWithClassName:@"UserPhoto"];
[userPhoto setObject:@"My trip to Hawaii!" forKey:@"imageName"];
[userPhoto setObject:imageFile forKey:@"imageFile"];
```

StackMob's powerful S3 integration allows for binary data to be attached to objects and saved to S3 seamlessly.

```obj-c
NSData *imageData = UIImageJPEGRepresentation(image, 0.7);
     
// Convert the binary data to string to save on Amazon S3
NSString *picData = [SMBinaryDataConversion stringForBinaryData:imageData name:@"image.jpg" contentType:@"image/jpg"];

NSMutableDictionary *userPhoto = [NSMutableDictionary dictionary];
[userPhoto setObject:picData forKey:@"image"];
[userPhoto setObject:@"Start from the bottom, now we're here!!" forKey:@"caption"];
```

<a href="http://developer.stackmob.com/tutorials/ios/Upload-to-S3" target="_blank">Check out our S3 tutorial for more information.</a>

# Push 

Here's how sending a push message looks with Parse:

```obj-c

NSDictionary *data = [NSDictionary dictionaryWithObjectsAndKeys:
    @"The Mets scored! The game is now tied 1-1!", @"alert",
    @"Increment", @"badge",
    @"cheering.caf", @"sound",
    nil];
PFPush *push = [[PFPush alloc] init];
[push setChannels:[NSArray arrayWithObjects:@"Mets", nil]];
[push setData:data];
[push sendPushInBackground];

```

StackMob's push messaging works similarly: 

```obj-c
NSMutableDictionary *pushMessage = [NSMutableDictionary dictionary];
NSArray *users = [NSArray arrayWithObject:@"Carl", @"Erick", @"Justin", nil];         
[pushMessage setValue:@"Hey, do you like Bon Jovi?" forKey:@"alert"];
[pushMessage setValue:@"ping.caf" forKey:@"sound"];

[self.pushClient sendMessage:pushMessage toUsers:users onSuccess:^{
    NSLog(@"Succesfully sent push message");
} onFailure:^(NSError *error) {
    NSLog(@"Error sending broadcast push message: %@", error);
}];
```

<a href="http://developer.stackmob.com/tutorials/ios/Push-Notifications" target="_blank">Check out our Push tutorial for more information.</a>

# Custom Code

With Parse's "Cloud Code", developers write endpoints in JavaScript, and call them from the client like so:

```obj-c
[PFCloud callFunctionInBackground:@"averageStars"
                   withParameters:@{@"movie": @"The Matrix"}
                            block:^(NSNumber *ratings, NSError *error) {
  if (!error) {
     
  }
}];
```

StackMob offers Custom Code, which allows developers to write powerful methods in either Java or Scala. Making a Custom Code request is simple:

```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
                                    initGetRequestWithMethod:@"averageStars"];

[request addQueryStringParameterWhere:@"movie" equals:@"The Matrix"];
         
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request onSuccess:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
    NSLog(@"Success: %@",JSON);
} onFailure:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON){
    NSLog(@"Failure: %@",error);
}];
```

<a href="http://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK" target="_blank">Check out our Custom Code tutorial</a> for more information.

# What Are You Waiting For?

There you have it, our overview is complete. Throughout this guide we showcased the similarities of the StackMob and Parse SDKs, and where necessary, highlighted the advantages of StackMob. So what are you waiting for? <a href="https://dashboard.stackmob.com/signup" target="_blank" class="btn btn-warning btn-rounded">Sign Up</a> and start building your mobile services on the StackMob platform.


