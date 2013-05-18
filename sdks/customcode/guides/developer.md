Custom Code Developer Guide
==========================================

To cover a universal audience, this developer guide will be covered in Java, though the fundamentals apply to all.  You can always find <a href="https://github.com/stackmob/stackmob-customcode-example" rel="nofollow">Scala and Clojure Custom Code examples</a> in our GitHub collection.

# Introduction

Custom Code is code you write that runs on the StackMob server in Java, Scala, and Clojure.  You deploy it to the servers by either uploading your JAR or linking your GitHub repository with StackMob.

If you wrote a method called `hello_world`, then StackMob creates an API endpoint for you at `https://api.stackmob.com/hello_world` so that it's accessible via the REST API with your OAuth 1.0 or OAuth 2.0 keys.

# Try it!

Interested in trying custom code without needing to write any yet?  You can either upload a ready-to-go JAR or link your app with a GitHub repository.

<span class="tab startcc" title="Upload a JAR"></span>

1.  Download this Custom Code JAR
2.  Upload it to StackMob

<span class="tab"></span>

<span class="tab startcc" title="Link with GitHub"></span>

<span class="tab"></span>

Now that Custom Code is running on StackMob, let's try it.

# Calling Custom Code

The StackMob client SDKs can call custom code via the REST API.

<span class="tab clientcall" title="iOS SDK"></span>
```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
	initGetRequestWithMethod:@"hello_world"];
         
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
  			  NSHTTPURLResponse *response, 
  			  id JSON) {
        NSLog(@"Success: %@",JSON);
  } onFailure:^(NSURLRequest *request, 
  				NSHTTPURLResponse *response, 
  				NSError *error, 
  				id JSON){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcall" title="Android SDK"></span>
```java
StackMob.getStackMob().getDatastore().get("hello_world", new StackMobCallback() {
    @Override public void success(String responseBody) {}
    @Override public void failure(StackMobException e) {}
});
```
<span class="tab"></span>

<span class="tab clientcall" title="JS SDK"></span>
```js
StackMob.customcode('hello_world', {}, 'GET', {
	success: function(result) {},
	error: function(result) {}
})
```
<span class="tab"></span>

You can easily interact with server code from the clients.

You can also use the Dashboard Console to call your API methods.

[Dashboard Console Screenshot]


# Declaring Custom Code

GitHub is the easiest way to get started with Custom Code.  Clone, download, or fork this repo to get started: <a href="https://github.com/stackmob/stackmob-customcode-java-examples" rel="nofollow">Java Custom Code examples</a>.  The necessary `pom.xml` and files are ready to go.

Let's add a `hello_world` method to your Custom Code so that clients can call it.  We'll put it under the path `src/main/java/com/stackmob/example`.

A method is represented by a class that extends the interface `CustomCodeMethod`, of which there are three methods to implement:

* `getMethodName`
* `getParams`
* `execute`

Let's look at the basic class.

```java,3,6,9
public class HelloWorld implements CustomCodeMethod {
  @Override
  public String getMethodName() { return "hello_world"; }

  @Override
  public List<String> getParams() { return Arrays.asList(); }

  @Override
  public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
    return new ResponseToProcess(HttpURLConnection.HTTP_OK, 
    	new HashMap<String, String>() {{
    		put("message", "hello world!");
    		}});
  }
}
```

This method will return JSON when called:

```js
{ message: "hello world!" }
```

`getMethodName` defines your API endpoint: <i>https://api.stackmob.com/</i>`hello_world`.

`getParams` whitelists the parameters you would pass into the method.  For REST API GET/DELETE requests, this would mean parameters from the URL.  For PUT/POST requests, these would be in the url-form-encoded body.

`execute` runs when the REST API point for `https://api.stackmob.com/hello_world` is hit.  The hashmap that is returned will translate to a JSON object in the response.

For StackMob to be aware of this method, you should add an entry for `HelloWorldExample` in `src/main/java/com/stackmob/example/EntryPointExtender.java`.

```java,5
public class EntryPointExtender extends JarEntryObject {
  @Override
  public List<CustomCodeMethod> methods() {
    List<CustomCodeMethod> list = new ArrayList<CustomCodeMethod>();
    list.add(new HelloWorld());
    return list;
  }

}
```

# Datastore

Custom Code can access your datastore via the Custom Code SDK (included in your project via Maven's `pom.xml` automatically).

<p class="alert">
	Custom Code bypasses access control permissions since it runs in your secure environment, so even if you have CRUD permissions set to <code>Not Allowed</code> in your schema permissions, Custom Code can access it.
</p>

You'll be accessing the datastore via `DataService`.

## Create

Let's create an object in the datastore from custom code.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {
	DataService ds = serviceProvider.getDataService();

	HashMap<String, Object> car = new HashMap<String, Object>();

	car.put("model", new SMString(model)); //string
    car.put("make", new SMString(make)); //string
    car.put("year", new SMInt(Long.parseLong(year))); //int

	try {
	  // This is how you create an object in the `car` schema
	  ds.createObject("car", new SMObject(car));
	} catch (InvalidSchemaException ise) {
	} catch (DatastoreException dse) {}
}
```

## Read

Let's read an object from the datastore.

```java
@Override
public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {

	DataService ds = serviceProvider.getDataService();
	List<SMCondition> query = new ArrayList<SMCondition>();
	Map<String, List<SMObject>> results = new HashMap<String, List<SMObject>>();

	try {
		//Get the car with ID "12345"	
		query.add(new SMEquals("car_id", new SMString("12345")));

		// Read objects from the car schema
		results.put("results", ds.readObjects("car", query))  

	} catch (InvalidSchemaException ise) {
	} catch (DatastoreException dse) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, results);
}		
```

## Update

Let's update an object.


```java
public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {

	DataService ds = serviceProvider.getDataService();
	List<SMUpdate> update = new ArrayList<SMUpdate>();
	update.add(new SMSet("year", new SMInt(Long.parseLong(year))));

	Map<String, SMValue> results = new HashMap<String, SMValue>();

	try {
		//Update the year of the car "12345"
		results.put("results", ds.updateObject("car", new SMString("12345"), update));
	} catch (InvalidSchemaException ise) {
	} catch (DatastoreException dse) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, results);
}
```

## Delete

Let's delete an object.


```java
public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {

	DataService ds = serviceProvider.getDataService();

	try {
	  ds.deleteObject("car", new SMString(carID));
	} catch (InvalidSchemaException ise) {
	} catch (DatastoreException dse) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<p class="alert alert-info">
	There are plenty of <a href="https://github.com/stackmob/stackmob-customcode-java-examples/tree/master/src/main/java/com/stackmob/example/CRUD" target="_blank" rel="nofollow">fully working Custom Code Datastore examples</a> available.  You can fork the repo and link it to your StackMob account.
</p>


# Queries

Let's make some queries against the datastore.

## Request Bodies and Parameters

Whether making queries or performing CRUD operations, you'll likely want to take user input to do so.

You can fetch parameters out of the URL for GET and DELETE requests.  You can fetch parameters out of the request body for PUT and POST requests.

### Fetching Parameters

Let's get the parameters out of the request URL.  To start out, let's first make a GET request from the client SDKs with a few parameters.

<span class="tab clientcallgetparams" title="iOS SDK"></span>
```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
	initGetRequestWithMethod:@"hello_world"];
         
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
  			  NSHTTPURLResponse *response, 
  			  id JSON) {
        NSLog(@"Success: %@",JSON);
  } onFailure:^(NSURLRequest *request, 
  				NSHTTPURLResponse *response, 
  				NSError *error, 
  				id JSON){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcallgetparams" title="Android SDK"></span>
```java
StackMob.getStackMob().getDatastore().get("hello_world", new StackMobCallback() {
    @Override public void success(String responseBody) {}
    @Override public void failure(StackMobException e) {}
});
```
<span class="tab"></span>

<span class="tab clientcallgetparams" title="JS SDK"></span>
```js
StackMob.customcode('hello_world', { name: 'joe', age: 10 }, 'GET', {
	success: function(result) {},
	error: function(result) {}
})
```
<span class="tab"></span>

These result in `https://api.stackmob.com/hello_world?name=joe&age=10`.

Now let's get the parameters out of the request in custom code.

```java,4,5
public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {

	String name = request.getParams().get("name"); //this will be a String
	String age = request.getParams().get("age"); //this will be a String

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

### Fetching JSON Body

Perhaps you're sending up JSON.  Let's do that with the client SDKs.

<span class="tab clientcallpostjson" title="iOS SDK"></span>
```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
	initGetRequestWithMethod:@"hello_world"];
         
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
  			  NSHTTPURLResponse *response, 
  			  id JSON) {
        NSLog(@"Success: %@",JSON);
  } onFailure:^(NSURLRequest *request, 
  				NSHTTPURLResponse *response, 
  				NSError *error, 
  				id JSON){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcallpostjson" title="Android SDK"></span>
```java
StackMob.getStackMob().getDatastore().get("hello_world", new StackMobCallback() {
    @Override public void success(String responseBody) {}
    @Override public void failure(StackMobException e) {}
});
```
<span class="tab"></span>

<span class="tab clientcallpostjson" title="JS SDK"></span>
```js
StackMob.customcode('hello_world', { name: 'joe', age: 10 }, 'POST', {
	success: function(result) {},
	error: function(result) {}
})
```
<span class="tab"></span>


These send up a request of:

```js
URL:
https://api.stackmob.com/hello_world

Body:

{name:'joe',age:10}

```

Let's see how we can pull that out of the JSON body.

```java,1,2,10
import org.json.JSONException;
import org.json.JSONObject;

...

public ResponseToProcess execute(ProcessedAPIRequest request, 
		SDKServiceProvider serviceProvider) {

	try {
	  JSONObject jsonObj = new JSONObject(request.getBody());
	  if (!jsonObj.isNull("places")) {
	  	places = Arrays.asList(jsonObj.getString("places").split(","));
	  }
	} catch (JSONException e) {
	  logger.error("Doh!  Problem parsing the JSON.", e);
	}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<p class="alert">
	StackMob returns <code>request.getBody()</code> as a plain String, so you'll need to mold it into the format you want - in this case JSON.
</p>

<p class="alert alert-info">
	Here's a full custom code class example of <a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/ReadParams.java" rel="nofollow">extracting JSON out of the request body</a>
</p>


## Fetching Multiple Results

At the heart of fetching objects is the `DataService`.  The `DataService` is retrieved from the request which is passed into the `execute` method.  To read several objects, you'd use the `readObjects` method.

```java,2,4
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	DataService ds = serviceProvider.getDataService();
	...
	results = ds.readObjects("car", query, 0, filters);
}
```

Let's look at `readObjects` more closely.  The method is overloaded, so let's take a look at the expanded one.

```java
List<SMObject> readObjects(String schema,
                           List<SMCondition> conditions,
                           int expandDepth,
                           ResultFilters resultFilters)
                           throws InvalidSchemaException,
                                  DatastoreException
```
We'll cover each of the parameters in more detail below, but here's an overview:

* `schema` - the name of schema you're querying
* `conditions` - the list of conditions which comprise the query (less than, greater than)
* `expandDepth` - the depth to which a query should be expanded for relationships (return full objects X levels deep)
* `resultFilters` - the options to be used when filtering the resultset (pagination, ordering)

## Equality

Let's look for users with the birthyear "2000".

```java,10
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	List<SMCondition> query = new ArrayList<SMCondition>();

	String year = "2000"

	DataService ds = serviceProvider.getDataService();
	List<SMObject> results;

	try {
	  query.add(new SMEquals("birthyear", new SMInt(Long.parseLong(year))));
	  results = ds.readObjects("user", query, 0, filters);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

## Comparison

You can query for greater than/less than.  Let's get all users with a `birthyear` field greater or equal to the year 2000.

```java,11
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	List<SMCondition> query = new ArrayList<SMCondition>();

	String year = "2000"

	DataService ds = serviceProvider.getDataService();
	List<SMObject> results;

	try {
	  // We only want years greater than or equal to the user input
	  query.add(new SMGreaterOrEqual("birthyear", new SMInt(Long.parseLong(year))));
	  results = ds.readObjects("user", query, 0, filters);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

## Array Queries

You can query for objects to see if an array or relationship contains a value.

Say a `user` had an array of `friends`, and you want to get those who are friends with `john` and `jane`.

```java,2-6
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	List<SMCondition> query = new ArrayList<SMCondition>();
	List<SMValue> values = new ArrayList<SMValue>(); 
	values.add(new SMString("john"));
	values.add(new SMString("jane"));
	query.add(new SMIn("friends", values));

	...

	DataService ds = serviceProvider.getDataService();
	List<SMObject> results;

	try {
	  results = ds.readObjects("user", query, 0, filters);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

This works for both array and relationship fields.

## Pagination Queries

Fetch a few results at a time.  Let's return items 5 through 9.

```java,3
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	List<SMCondition> query = new ArrayList<SMCondition>();

	ResultFilters filters = new ResultFilters(0, 9, null, null);

	DataService ds = serviceProvider.getDataService();
	List<SMObject> results;

	try {
	  results = ds.readObjects("user", query, 0, filters);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```



<p class="alert alert-info">
	<a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/PaginateResults.java" rel="nofollow">Pagination Example</a>
</p>

## Ordering

You can sort results and even provide tie breakers.  We are going to primarily sort by year (oldest to most recent) and then by createddate from newest to oldest.

Pass `SMOrdering` specifiers into `ResultFilters`.


```java,2-5
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	List<SMOrdering> orderings = Arrays.asList(
	        new SMOrdering("year", OrderingDirection.ASCENDING),
	        new SMOrdering("createddate", OrderingDirection.DESCENDING));
	ResultFilters filters = new ResultFilters(0, -1, orderings, null); //don't limit the number of results, so set to -1

	DataService ds = serviceProvider.getDataService();
	List<SMObject> results;

	try {
	  query.add(new SMGreaterOrEqual("year", new SMInt(Long.parseLong(year))));
	  results = ds.readObjects("user", query, 0, filters);

	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```


# Relationships

Just as in our SDKs, you can manipulate relationships from the Custom Code SDK.

Let's assume we have a `user` schema, and we've related the user to the `car` schema.  Let's add cars to the user.

[Screenshot of Relationship between User and Car]

## Adding related objects

**Existing Objects**

If the `car` already exists in the datastore, and you want to relate it with a `user`.  You just need to relate the two existing objects together with the primary keys.

```java,9
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	DataService ds = serviceProvider.getDataService();
	
	List<SMValue> relatedObjects = new ArrayList<SMValue>();
	relatedObjects.add(new SMString("Camry")); //primary keys of car objects
	relatedObjects.add(new SMString("Accord"));

	try {
	  SMObject result = ds.addRelatedObjects("user", new SMString("john"), "garage", valuesToAppend);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

No new objects are created in the datastores.  We're just linking existing objects with each other.

**New Objects**

If the cars *don't* exist in the `car` schema yet, we can create them and relate them to the `user` in one call.  Let's give a `user` two new `cars`.  The following will create the two `car` objects in the respective `car` schema and relate them to the user at the same time.

```java,23,24
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	// These are some example cars that will be created
	Map<String, SMValue> carValues1 = new HashMap<String, SMValue>();
	carValues1.put("make", new SMString("Audi"));
	carValues1.put("model", new SMString("R8"));
	carValues1.put("year", new SMInt(2005L));

	Map<String, SMValue> carValues2 = new HashMap<String, SMValue>();
	carValues2.put("make", new SMString("Audi"));
	carValues2.put("model", new SMString("spyder"));
	carValues2.put("year", new SMInt(2005L));

	SMObject car1 = new SMObject(carValues1);
	SMObject car2 = new SMObject(carValues2);

	List<SMObject> cars = new ArrayList<SMObject>();
	cars.add(car1);
	cars.add(car2);

	DataService ds = serviceProvider.getDataService();

	try {
	  BulkResult result = ds.createRelatedObjects(
	  	"user", new SMString("john"), "cars", cars);

	  feedback.put(owner + " now owns", cars);

	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

The user should now look something like:

```js
{
	username: 'john',
	cars: ["151gd", "1351dg5"],
	...
}
```

[Talk about the difference between BulkResult and List<SMObject> ]]

## Fetching related objects

To retrieve related objects from the datastore, you can simply call `readObjects`.  Normally the related objects are represented by the primary keys:

```js
{
	username: 'john',
	cars: ['Camry', 'Accord']
}
```

But you can get **expanded related objects** by passing an expand depth of 1.

```js
{
	username: 'john',
	cars: [{
		car_id: 'Camry',
		maker: 'Toyota',
		...
	}, {
		car_id: 'Accord',
		maker: 'Honda',
		...
	}]
}
```

Here's the code.

```java,3,5,6
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	DataService ds = serviceProvider.getDataService();
	var expandDepth = 1;
	try {
		List<SMObject> results = ds.readObjects("user", 
			new ArrayList<SMCondition>(), expandDepth);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

# Authentication

The StackMob client SDKs support OAuth 2.0 login.  When they make a request to custom code, custom code is aware of the logged in user.

```java,2
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	if (request.getLoggedInUser() != null) loggedInAction();
	else notLoggedInAction();
	...
	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

* We currently don't have a way in custom code to generate OAuth 2.0 tokens at this time.

# Geolocation

## Persisting Geopoints

To manipulate geolocations in Custom Code, we'll just prepare the `lat` and `long` values as `SMDouble` instances.  We'll then pass them as a `SMSet` into our CRUD operations.

Let's update `john`'s home with a new geolocation value.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {

	DataService ds = serviceProvider.getDataService();
	
	Map<String, SMValue> geoPoint = new HashMap<String, SMValue>();
	geoPoint.put("lat", new SMDouble(new Double(37.772201));
	geoPoint.put("lon", new SMDouble(new Double(-122.406326));

	List<SMUpdate> update = new ArrayList<SMUpdate>();
	update.add(new SMSet("home", new SMObject(geoPoint)));

	try {
	  SMObject result = ds.updateObject("user", new SMString("john"), update);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<p class="alert alert-info">
	<a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/geopoints/WriteGeo.java">Write Geo</a>
</p>



## Querying Geopoints

Let's query for several users who live within ~60 miles of us.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {

    SMNear near = new SMNear(           // Near-condition results will always be sorted by distance
            "position",                 // name of GeoField in schema
            new SMDouble(37.77207),     // latitude
            new SMDouble(-122.40621),   // longitude
            new SMDouble(.0025));       // radius - (62.25 mi) can be null

    SMWithinBox withinBox = new SMWithinBox(  // Whereas withinbox results can be sorted
            "position",
            new SMDouble(37.8),
            new SMDouble(-122.47),      // Top Left coords
            new SMDouble(37.7),
            new SMDouble(-122.3));      // Bottom Right coords

    DataService ds = serviceProvider.getDataService();

    List<SMCondition> query = new ArrayList<SMCondition>();
    query.add(near);
    query.add(withinBox);
    

    try {
      List<SMObject> results = ds.readObjects("user", query);
    } catch (Exception e) {}

    return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
  }
```
<p class="alert">
	StackMob geolocation distances are in radians.  .0025 radians is around 62.25 miles.
</p>

<p class="alert alert-info">
	<a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/geopoints/ReadGeo.java">Reading Geo</a>
</p>


# Push

## Registering Devices

Let's pass a `device_token` to the method and register it.  Here, we'll register it to a particular username so that in the future, we can send push messages to StackMob usernames rather than device tokens.  That'll make things a bit easier.

Because StackMob supports both Apple and Google Push notifications, you need to specify the type as well.

```java,3,8,11,12
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	//Specify the token type
	TokenType deviceTokenType = TokenType.iOS;

	//Can also be TokenType.AndroidGCM or TokenType.Android (the latter is deprecated)

	String deviceToken = request.getParams().get("device_token"); //device token 
	TokenAndType token = new TokenAndType(deviceToken, deviceTokenType); // token type can be iOS or GCM

	try {
	  PushService service = serviceProvider.getPushService();
	  service.registerTokenForUser(username, token);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

You've now registered the device token so that StackMob can send messages to it.

<a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/SMPushRegisterDevice.java" rel="nofollow">Register Device example</a>

## Broadcast Messages

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	Map<String, String> payload = new HashMap<String, String>();

	try {
	  PushService ps = serviceProvider.getPushService();
	  // Add data to your push payload
	  payload.put("key1", "value1");
	  payload.put("sound", "someSound.mp3");
	  payload.put("alert", "Push Alert!");

	  ps.broadcastPush(payload);

	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/BroadcastPushNotification.java

## Direct Messages

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	Map<String, String> payload = new HashMap<String, String>();

	try {
	  PushService ps = serviceProvider.getPushService();

	  // Add data to your payload
	  payload.put("badge", "1");
	  payload.put("key1", "some data");

	  // Send the payload to the specified user
	  ps.sendPushToUsers(Arrays.asList("john"),payload);

	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/DirectPushNotification.java



# Logging

You can write logs that you can view at <a href="https://dashboard.stackmob.com/data/logs">your Dashboard Logs</a>.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	LoggerService logger = serviceProvider.getLoggerService(Logging.class);

	logger.info("This is an INFO log");
	logger.info("This is ")
	
	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<p class="alert alert-info">
	<a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/Logging.java" rel="nofollow">A full example of writing logs.</a>
</p>

# External HTTP Calls

You can make calls to external APIs from custom code, but for security purposes, they must go through our HTTP call maker.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	// The service you're going to be using
	String url = "http://www.httpbin.org/get";

	// Formulate request headers
	Header accept = new Header("Accept-Charset", "utf-8");
	Header content = new Header("Content-Type", "application/x-www-form-urlencoded");

	Set<Header> set = new HashSet();
	set.add(accept);
	set.add(content);

	try {
	  HttpService http = serviceProvider.getHttpService();

	  /* In this Example we are going to be making a GET request
	   * but PUT/POST/DELETE requests are also possible.
	   */
	  GetRequest req = new GetRequest(url,set);
	  HttpResponse resp = http.get(req);

	  responseCode = resp.getCode();
	  responseBody = resp.getBody();

	} catch (Exception e) {}

	return new ResponseToProcess(responseCode, ...);
}
```

<p class="alert alert-info">
	Here's a basic <a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/HttpRequest.java" rel="nofollow">custom code class example of making an external API call</a>.
</p>

# Custom Headers and Response

You can send up custom headers in your custom code API calls.

You can also send back custom response headers and response bodies in various formats (non-JSON).

# External Dependencies

If you're working with external libraries, you can include them with Maven or include the JAR.

## Maven

Maven helps you build your projects by also organizing your dependencies.  Many developers upload their JARs to Maven's central repository, allowing you, the developer, to simply define what resource you need in Maven's xml.  Maven will pull it in for you automatically to help build your project.


```xml

```

Now from the command line, run `maven compile`.  Maven will start building your project and finally output your Custom Code JAR that you'll upload to StackMob.

<p class="alert alert-info">
	Learn more about Maven and how to find JARs in Maven.
</p>

## JARs

To include JARs


# Deploying Code

You have two ways of getting your code running on StackMob.  You can upload a JAR that you've built

## GitHub

Instructions and Dashboard

## JAR

Instructions and Dashboard

# Restrictions

## Security Manager

## External API calls

# Testing Locally

how to test the custom code locally (local runner)

# Best Practices
