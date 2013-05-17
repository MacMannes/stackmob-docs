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

<p class="alert alert-info">
	There are plenty of <a href="https://github.com/stackmob/stackmob-customcode-java-examples/tree/master/src/main/java/com/stackmob/example/CRUD" target="_blank" rel="nofollow">fully working Custom Code Datastore examples</a> available.  You can fork the repo and link it to your StackMob account.
</p>


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


# Queries

Let's make some queries against the datastore.

## Request Bodies and Parameters

Whether making queries or performing CRUD operations, you'll likely want to take user input to do so.

You can fetch parameters out of the URL for GET and DELETE requests.  You can fetch parameters out of the request body for PUT and POST requests.

### Fetching Parameters

Let's get the parameters out of the request URL.  To start out, let's first make a GET request from the client SDKs with a few parameters.

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

### Fetching Body

Perhaps you've sent up form url encoded parameters in your POST and PUT calls.  The code's the same as above.

### Fetching JSON Body

Perhaps you're sending up JSON.  Let's do that with the client SDKs.

Let's see how we can pull that out of the JSON body.

```java,1,2,9
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

<p class="alert alert-info">
	Here's a full custom code class example of <a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/ReadParams.java" rel="nofollow">extracting JSON out of the request body</a>
</p>


## Equality

You can query against the datastore with the Custom Code SDK.

Let's look for some objects with equality comparisons.

```java
```

## Comparison

You can query for greater than/less than.

## Array Queries

You can query for objects to see if an array or relationship contains a value.

## Pagination Queries

Fetch a few results at a time.  Let's return items 5 through 9.

# Relationships

StackMob supports relationships.

## Adding related objects


## Fetching related objects

# Authentication

The StackMob client SDKs support OAuth 2.0 login.  When they make a request to custom code, custom code is aware of the logged in user.

```java
```

# External HTTP Calls

You can make calls to external APIs from custom code, but for security purposes, they must go through our HTTP call maker.

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
