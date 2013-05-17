Custom Code Developer Guide
==========================================

To cover a universal audience, this developer guide will be covered in Java, though the fundamentals apply to all.  You can always find <a href="https://github.com/stackmob/stackmob-customcode-example" rel="nofollow">Scala and Clojure Custom Code examples</a> in our GitHub collection.

# Introduction

Custom Code is code you write that runs on the StackMob server in Java, Scala, and Clojure.  You deploy it to the servers by either uploading your JAR or linking your GitHub repository with StackMob.

If you wrote a method called `hello_world`, then StackMob creates an API endpoint for you at `https://api.stackmob.com/hello_world` so that it's accessible via the REST API with your OAuth 1.0 or OAuth 2.0 keys.

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

```java
```

## Request Bodies and Parameters

Whether making queries or performing CRUD operations, you'll likely want to take user input to do so.

You can fetch parameters out of the URL for GET and DELETE requests.  You can fetch parameters out of the request body for PUT and POST requests.

### Fetching Parameters

Let's get the parameters out of the request URL.  Let's make a GET request from the SDKs with a few parameters.

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

And now let's get the parameters out of the request.

```java
```

### Fetching Body

Perhaps you've sent up form url encoded parameters in your POST and PUT calls.  The code's the same as above.

### Fetching JSON Body

Perhaps you're sending up JSON.  Let's see how we can pull that out of the JSON body.

```java
```


## Equality

Just as with Stackmob's client SDKs, you can query against the datastore with the Custom Code SDK.

Let's look for some objects with equality comparisons.

```java
```

## Comparison

You can query for greater than/less than.

## Array Queries

in

## Pagination Queries

how to paginate

# Relationships

how does the custom code deal with relationships

# Authentication

checking logged in user

# External HTTP Calls

how to make external http calls

# Custom Headers and Response

reading custom headers and writing custom responses (non JSON)

# External Dependencies

how to include external dependencies

## Maven

## JARs

# Security Manager

Limitations of Custom Code

# Deploying Code

## GitHub

Instructions and Dashboard

## JAR

Instructions and Dashboard

# Testing Locally

how to test the custom code locally (local runner)

# Best Practices
