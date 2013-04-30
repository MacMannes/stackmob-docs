# iOS: Parse to StackMob Tutorial

<a href="https://www.stackmob.com/parse/">&larr; Back to Migrate from Parse</a>

# Introduction

If you're an iOS developer looking to switch from Parse, have no fear; we'll provide a quick comparison of features from their SDK to those found in the StackMob iOS SDK. We'll also highlight some of the advantages StackMob presents. From the code examples, you'll see that making the switch to StackMob is really easy.

Our number one goal is to create a better experience for developers, which is why the StackMob iOS SDK utilizes powerful Core Data integration. 

Core Data allows us to place a familiar wrapper around StackMob REST calls and Datastore API, while getting all the benefits of Core Data's focus on high database performance and low memory usage. iOS developers can leverage their existing knowledge of Core Data to quickly integrate StackMob into their applications. 

The integration also features a caching system so previously fetched data can be available to users when their device does offline.

For the full SDK reference, check out our <a href="http://stackmob.github.io/stackmob-ios-sdk/">iOS SDK Reference</a>. If you're new to Core Data, <a href="http://go.stackmob.com/learncoredata.html">check out our Core Data eBook.</a> For those interested in sticking to the REST-based way of making requests, we provide the full Datastore API as well.

We have taken the time to get a base project ready for you so all you need to do is edit your object model.  You can <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip">download the project here</a>.

# Getting Started

You initialize the Parse SDK using both your public and private keys:

```obj-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

  [Parse setApplicationId:@YOUR_PUBLIC_KEY clientKey:@YOUR_PRIVATE_KEY];

  ...

}
```

With StackMob, you initialize an instance of `SMClient`, using only your public key, as well as an instance of `SMCoreDataStore`, where you can choose all the settings you want for the Core Data integration:

```obj-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

  // Declare a variable in your AppDelegate.h file called SMClient *client;
  self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
  self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.manangedObjectModel];

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

With StackMob, you simply create a new Managed Object, and call save on the Managed Object Context:

```obj-c

NSManagedObjectContext *context = self.coreDataStore contextForCurrentThread];
NSManagedObject *gameScore = [NSEntityDescription insertNewObjectForEntityForName:@"Gamescore" inManagedObjectContext:context];
[gameScore setValue:[gameScore assignObjectId] forKey:[gameScore primaryKeyField]];
[gameScore setValue:[NSNumber numberWithInt:1337] forKey:@"score"];
[gameScore setValue:@"Sean Plott" forKey:@"playerName"];
[gameScore setValue:[NSNumber numberWithBool:NO] forKey:@"cheatMode"];

[context saveOnSuccess:^{
    // The gameScore saved successfully.
} onFailure:^(NSError *error) {
    // There was an error saving the gameScore.
}];
```

As you can see, we've provided an asynchronous version of the existing Core Data save method, so network requests can work in the background without inhibiting your UI.

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

Here, we simply execute a Core Data fetch request:

```obj-c
NSFetchRequest *playerFetch = [[NSFetchRequest alloc] initWithEntityName:@"Gamescore"];
[playerFetch setPredicate:[NSPredicate predicateWithFormat:@"playerName == 'Carl Atupem'"]];

NSManagedObjectContext *context = self.coreDataStore contextForCurrentThread];
[context executeFetchRequest:playerFetch onSuccess:^(NSArray *results) {

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

With StackMob, you can create users the same way you would regular Core Data objects, except that the User entity `NSManagedObject` subclass will inherit from `SMUserManagedObject`, giving you secure methods to save your password:

```obj-c
NSManagedObjectContext *context = self.coreDataStore contextForCurrentThread];
User *user = [NSEntityDescription insertNewObjectForEntityForName:@"User" inManagedObjectContext:context];
[user setUsername:@"Carl Atupem"];
[user setPassword:@"STACKMOBRULEZ123"];
[user setEmail:@"carl_atupem@aol.com"];
[user setPhone:@"617-666-1234"];

[context saveOnSuccess:^{
    // The user object saved successfully.
} onFailure:^(NSError *error) {
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

You'll be able to save your GeoPoint objects directly through Core Data, too.  Declare the attribute as Transformable and StackMob takes care of all the translation.

Check out our <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data" target="_blank">Saving Geo Location Data</a> and <a href="https://developer.stackmob.com/tutorials/ios/Querying-Geo-Location-Data" target="_blank">Querying Geo Location Data</a> tutorials for more information.

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

// Save to an existing User managed object, with string attributes `image` and `imageCaption`:
User *user = self.user;
[user setImage:picData];
[user setImageCaption:@"Start from the bottom, now we're here!!"];
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

<a href="http://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK" target="_blank">Check out our Custom Code tutorial for more information.</a>

# What Are You Waiting For?

There you have it, our overview is complete. Throughout this guide we showcased the similarities of the StackMob and Parse SDKs, and where necessary, highlighted the advantages of StackMob. So what are you waiting for? <a href="https://dashboard.stackmob.com/signup" target="_blank" class="btn btn-warning btn-rounded">Sign Up</a> and start building your mobile services on the StackMob platform.


