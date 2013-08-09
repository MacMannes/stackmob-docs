Custom Code Developer Guide
==========================================

## Overview

Custom Code is Java, Scala, or Clojure code you write that runs on the StackMob server.  You deploy it to the servers by either uploading your JAR or linking your GitHub repository with StackMob.

If you wrote a Custom Code method called `hello_world`, then StackMob creates an API endpoint for you at `https://api.stackmob.com/hello_world` so that it's callable from the REST API and StackMob Client SDKs.  The server will execute the server side custom code and return a response with a body that you define.

Example of mobile SDKs calling Custom Code:

<span class="tab clientcall" title="iOS SDK"></span>

<p><b>iOS calling Custom Code <code>hello_world</code></b></p>

```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
  initGetRequestWithMethod:@"hello_world"];
         
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
          NSHTTPURLResponse *response, 
          id responseBody) {
        NSLog(@"Success: %@",responseBody);
  } onFailure:^(NSURLRequest *request, 
          NSHTTPURLResponse *response, 
          NSError *error, 
          id responseBody){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcall" title="Android SDK"></span>

<p><b>Android calling Custom Code <code>hello_world</code></b></p>

```java
StackMob.getStackMob().getDatastore().get("hello_world", new StackMobCallback() {
    @Override public void success(String responseBody) {}
    @Override public void failure(StackMobException e) {}
});
```
<span class="tab"></span>

<span class="tab clientcall" title="JS SDK"></span>

<p><b>JS calling Custom Code <code>hello_world</code></b></p>

```js
StackMob.customcode('hello_world', {}, 'GET', {
  success: function(result) {},
  error: function(result) {}
})
```
<span class="tab"></span>

<span class="tab clientcall" title="REST API"></span>

<p><b>REST API format for Custom Code <code>hello_world</code></b></p>

```js
URL: 
GET https://api.stackmob.com/hello_world

Request Headers:
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: /* Your Public Key */

Request Body:
None
```
<span class="tab"></span>

Custom Code allows you to easily communicate between client and server.

In each section of this guide you may see colored boxes which are meant to highlight important information:

<p class="alert">Gold boxes call out warnings, gotchas, and information we don't want you to miss.</p>

<p class="alert alert-info">Blue boxes contain links to sections in the full API reference, as well as full working projects for you to download and in-depth tutorials for you to read through.</p>

To cover a universal audience, this developer guide will be covered in Java, though the fundamentals apply to each supported language of Scala and Clojure as well.

Code examples of the topics covered in this Developer Guide can be found in our GitHub repositories below.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples">StackMob Custom Code Java examples on <i class="icon-github"></i> GitHub</a></li>
        <li><a href="https://github.com/stackmob/stackmob-customcode-example" target="_blank">StackMob Custom Code Scala and Clojure examples on <i class="icon-github"></i> GitHub</a></li>
      </ul>
    </div>
  </div>
</div>

### Try Custom Code in less than 5 minutes

We've provided a ready-to-upload JAR so you can try Custom Code immediately. It will introduce a `hello_world` function on the server side that, when called, returns JSON `{ msg: "Hello, World!"}`.

1. <a href="https://marketplace.stackmob.com/module/customcode" target="_blank">Add the Custom Code module</a>.

2. <a href="https://s3.amazonaws.com/static.stackmob.com/customcode/stackmob-customcode-build-0.1.0-SNAPSHOT.jar" class="btn btn-info">Download the Hello World example JAR</a>

3. <a href="https://dashboard.stackmob.com/module/customcode/upload" target="_blank">Upload it through the Dashboard</a>.

That's it!  You can now try Custom Code in the <a href="https://dashboard.stackmob.com/data/console?method=hello_world&type=cm&avn=0" target="_blank">Dashboard Console</a>, a UI to help you make REST API requests against your StackMob API (and hence your custom code).

Below is the result of selecting the `hello_world` endpoint and submitting a `GET` request:

<p class="screenshot"><a href="https://dashboard.stackmob.com/data/console?method=hello_world&type=cm&avn=0" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/console/dashboard-console-customcode-helloworld-overview.png" alt=""/></a></p>

Notice that you're making a REST API call to `hello_world`.  Custom Code methods are converted into REST API endpoints and are accessible from anywhere you can make an HTTP request with headers.

You just uploaded and ran your first Custom Code method!

<p class="alert">
  Custom Code methods are converted into REST API endpoints and are accessible from anywhere you can make an HTTP request with headers.
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-starter" target="_blank" rel="nofollow">View the example Custom Code Java Starter project</a></li>
      </ul>
    </div>
  </div>
</div>


## Setup

Let's get a Custom Code project started so that you can start adding your own methods (API endpoints).  We've provided an easy way to get started below.  It's a zip file with all the necessary files needed to build a project.  (It's a <a href="http://maven.apache.org/" rel="nofollow" target="_blank">Maven</a> project, so be sure to <a href="http://maven.apache.org/download.cgi#Installation" target="_blank" rel="nofollow">install Maven</a>.)

<a href="https://github.com/stackmob/stackmob-customcode-java-starter/archive/master.zip" class="btn btn-info">Download the Custom Code Java Starter Template zip</a> and unzip it.

If you want to build your JAR right away, simply run `mvn package` in the root folder (where `pom.xml` is).

We've included `src/main/java/com/stackmob/customcode/HelloWorld.java` which you can remove.  But it's there as an example, and we'll use it here in this Developer Guide. (Note it's also referred to in `EntryPointExtender.java`)

<p class="alert">
  Use GitHub?  You can skip Maven and JARs.  Fork the <a href="https://github.com/stackmob/stackmob-customcode-java-starter">StackMob Custom Code Java Starter</a> repository and <a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank">link your Custom Code repository with StackMob</a>.  StackMob will automatically build your project for you.
</p>


### Class Template

A method is represented by a class that extends the interface `CustomCodeMethod`, of which there are three methods to implement:

* `getMethodName`
* `getParams`
* `execute`

Let's look at the basic class, represented by `HelloWorld.java`.

```java,4,9,12
public class HelloWorld implements CustomCodeMethod {

  @Override
  public String getMethodName() { 
    return "hello_world"; //no dashes or spaces allowed
  }

  @Override
  public List<String> getParams() { return new ArrayList<String>(); }

  @Override
  public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
    Map<String, Object> map = new HashMap<String, Object>();
    map.put("msg", "Hello, world!");
    return new ResponseToProcess(HttpURLConnection.HTTP_OK, map);
  }

}
```

This method will return JSON when called:

```js
{ msg: "Hello, world!" }
```

`getMethodName` defines your API endpoint: <i>https://api.stackmob.com/</i>`hello_world`.

`getParams` whitelists the parameters you would pass into the method from the URL.  (It does not fetch parameters from the POST body because StackMob custom code doesn't support www-url-form-encoded requests at this time)

`execute` runs when the REST API point for `https://api.stackmob.com/hello_world` is hit.  The hashmap that is returned will translate to a JSON object in the response.

<p class="alert">
  <i class="icon-warning-sign"></i> <b>getMethodName</b>: we do not support dashes (`-`) in method names, but we do support underscores (`_`).
</p>

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/core/customcode/CustomCodeMethod.html" target="_blank">CustomCodeMethod</a></li>
      </ul>
    </div>
  </div>
</div>

### Exposing the Method

For StackMob to be aware of this method, you should add an entry for `HelloWorld` in `src/main/java/com/stackmob/customcode/EntryPointExtender.java`.  **This is a mandatory step.**

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

<p class="alert">
  <i class="icon-warning-sign"></i> Methods <b>must be entered</b> in <code>EntryPointExtender</code> in order for StackMob to discover them.
</p>

## Datastore

Custom Code can access your datastore via the Custom Code SDK (included in your project via Maven's `pom.xml` automatically).

<p class="alert">
	Custom Code bypasses access control permissions since it runs in your secure environment, so even if you have CRUD permissions set to <code>Not Allowed</code> in your schema permissions, Custom Code can access it.
</p>

You'll be accessing the datastore via `DataService`.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>Examples</strong>
      <ul>
        <li>There are several <a href="https://github.com/stackmob/stackmob-customcode-java-examples/tree/master/src/main/java/com/stackmob/example/CRUD" target="_blank" rel="nofollow">fully working Custom Code Datastore examples</a> available.</li>
      </ul>
    </div>
  </div>
</div>

### Create

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#createObject(java.lang.String, com.stackmob.sdkapi.SMObject)" target="_blank">DataService#createObject</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/CreateObject.java" target="_blank">Create Object Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Read

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/ReadObject.java" target="_blank">Read Object Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Update

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#updateObject(java.lang.String, com.stackmob.sdkapi.SMValue, java.util.List, java.util.List)" target="_blank">DataService#updateObject</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/UpdateObject.java" target="_blank">Update Object Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Delete

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#deleteObject(java.lang.String, com.stackmob.sdkapi.SMValue)" target="_blank">DataService#deleteObject</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/DeleteObject.java" target="_blank">Delete Object Example</a></li>
      </ul>
    </div>
  </div>
</div>

## Request Data

Whether making queries or performing CRUD operations, you'll likely want to take user input to do so.

You can:

* fetch parameters out of the URL for GET and DELETE requests.  
* fetch data/JSON out of the request body for PUT and POST requests.

### Fetching Parameters

Let's get the parameters out of the request URL.  To start out, let's first make a GET request from the client SDKs with a few parameters.

<span class="tab clientcallgetparams" title="iOS SDK"></span>
```obj-c
SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
    initGetRequestWithMethod:@"hello_world"];
 
[request addQueryStringParameterWhere:@"name" equals:@"joe"];
[request addQueryStringParameterWhere:@"age" equals:[NSNumber numberWithInt:10]];
          
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
              NSHTTPURLResponse *response, 
              id responseBody) {
        NSLog(@"Success: %@",responseBody);
  } onFailure:^(NSURLRequest *request, 
                NSHTTPURLResponse *response, 
                NSError *error, 
                id responseBody){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcallgetparams" title="Android SDK"></span>
```java
List<Map.Entry<String, String>> args = new ArrayList<Map.Entry<String, String>>();
args.add(new AbstractMap.SimpleEntry<String, String>("name","joe"));
args.add(new AbstractMap.SimpleEntry<String, String>("age","10"));
 
List<Map.Entry<String, String>> headers = new ArrayList<Map.Entry<String, String>>();
 
StackMobDatastore ds = client.getDatastore();
ds.get("hello_world", args, headers, new StackMobCallback() {});
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
 
NSDictionary *body = [NSDictionary dictionaryWithObjectsAndKeys:@"joe", @"name", [NSNumber numberWithInt:10], @"age"];
NSErrtor *error = nil;
NSData* jsonData = [NSJSONSerialization dataWithJSONObject:body 
  options:NSJSONWritingPrettyPrinted error:&error];
if (error) {
  // Handle error
}
 
request.requestBody = [[NSString alloc] initWithData:jsonData enconding:NSUTF8StringEncoding];
 
[[[SMClient defaultClient] dataStore] performCustomCodeRequest:request 
  onSuccess:^(NSURLRequest *request, 
              NSHTTPURLResponse *response, 
              id responseBody) {
        NSLog(@"Success: %@",responseBody);
  } onFailure:^(NSURLRequest *request, 
                NSHTTPURLResponse *response, 
                NSError *error, 
                id responseBody){
        NSLog(@"Failure: %@",error);
}];
```
<span class="tab"></span>

<span class="tab clientcallpostjson" title="Android SDK"></span>

```java
//POST /hello_world with body {name:'joe',age:10}
Map<String, String> body = new HashMap<String, String>();
body.put("name", "joe");
body.put("age", "10");
ds.post("hello_world", body, new StackMobCallback() {});
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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span12">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/ReadParams.java" target="_blank">Extracting JSON out of the request body example</a></li>
      </ul>
    </div>
  </div>
</div>




## Queries

Let's make some queries against the datastore.

### Fetching Multiple Results

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
* `expandDepth` - the depth to which a query should be expanded for relationships (return full objects X levels deep) - we'll cover this later in the Relationships section.  For now, you can assume this value can be 0.
* `resultFilters` - the options to be used when filtering the resultset (pagination, ordering)


<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List, int)" target="_blank">DataService#readObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/ReadAllObjects.java" target="_blank">Read All Objects Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Equality

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
      	<li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMEquals.html" target="_blank">SMEquals</a></li>
      </ul>
    </div>
  </div>
</div>

### Comparison

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
      	<li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMGreater.html" target="_blank">SMGreater</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMGreaterOrEqual.html" target="_blank">SMGreaterOrEqual</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMLess.html" target="_blank">SMLess</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMLessOrEqual.html" target="_blank">SMLessOrEqual</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMNotEqual.html" target="_blank">SMNotEqual</a></li>
        <li>there are more in the docs!</li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/QueryByEquality.java" target="_blank">Comparison Query Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Array Queries

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
      	<li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMIn.html" target="_blank">SMIn</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMValue.html" target="_blank">SMValue</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMInt.html" target="_blank">SMInt</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMLong.html" target="_blank">SMLong</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMString.html" target="_blank">SMString</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMBoolean.html" target="_blank">SMBoolean</a></li>
      </ul>
    </div>
  </div>
</div>

### Pagination Queries

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/ResultFilters.html" target="_blank">ResultFilters</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/PaginateResults.java" target="_blank">Pagination Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Ordering

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMOrdering.html" target="_blank">SMOrdering</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/CRUD/QueryByEquality.java" target="_blank">Equality Query with Ordering Example</a></li>
      </ul>
    </div>
  </div>
</div>

## Relationships

Just as in our SDKs, you can manipulate relationships from the Custom Code SDK.

Let's assume we have a `user` schema, and we've related the user to the `car` schema.  Let's add cars to the user on the `garage` field, which we've created as a Relationship field.

<p class="screenshot"><a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/dashboard/tutorials/schemas/dashboard-schemas-relationships-user-car-garage.png" alt=""/></a></p>

### Adding related objects

**Existing Child Object Instances**

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#addRelatedObjects(java.lang.String, com.stackmob.sdkapi.SMValue, java.lang.String, java.util.List)" target="_blank">DataService#addRelatedObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/relations/RelateToParent.java" target="_blank">Add Existing Object to Parent Example</a></li>
      </ul>
    </div>
  </div>
</div>

**New Child Object Instances**

If the cars *don't* exist in the `car` schema yet, we can create them and relate them to the `user` in one call.  Let's give a `user` two new `cars`.  The following will create the two `car` objects in the respective `car` schema and relate them to the user at the same time.

```java,23,24
public ResponseToProcess execute(ProcessedAPIRequest request, 
      SDKServiceProvider serviceProvider) {
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
	  	"user", new SMString("john"), "garage", cars);

	  feedback.put(owner + " now owns", cars);

	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

The user should now look something like:

```js
{
	username: 'john',
	garage: ["151gd", "1351dg5"],
	...
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#createRelatedObjects(java.lang.String, com.stackmob.sdkapi.SMValue, java.lang.String, java.util.List)" target="_blank">DataService#createRelatedObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/relations/OneStepCreateRelate.java" target="_blank">Create and Add New Object to Parent Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Fetching related objects

To retrieve related objects from the datastore, you can simply call `readObjects`.  Normally the related objects are represented by the primary keys:

```js
{
	username: 'john',
	garage: ['Camry', 'Accord']
}
```

But you can get **expanded related objects** by passing an expand depth of 1.

```js
{
	username: 'john',
	garage: [{
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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List, int)" target="_blank">DataService#readObjects</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/relations/FetchExpand.java" target="_blank">Expanded Fetch Example</a></li>
      </ul>
    </div>
  </div>
</div>

## Authentication

The StackMob client SDKs support OAuth 2.0 login.  When they make a request to custom code, custom code is aware of the logged in user.

```java,2
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	if (request.getLoggedInUser() != null) loggedInAction();
	else notLoggedInAction();
	...
	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/core/rest/ProcessedAPIRequest.html#getLoggedInUser()" target="_blank">ProcessedAPIRequest#getLoggedInUser</a></li>
      </ul>
    </div>
  </div>
</div>

## Geolocation

### Persisting Geopoints

To manipulate geolocations in Custom Code, we'll just prepare the `lat` and `long` values as `SMDouble` instances.  We'll then pass them as a `SMSet` into our CRUD operations.

Let's update `john`'s home with a new geolocation value.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {

	DataService ds = serviceProvider.getDataService();
	
	Map<String, SMValue> geoPoint = new HashMap<String, SMValue>();
	geoPoint.put("lat", new SMDouble(37.772201));
	geoPoint.put("lon", new SMDouble(-122.406326));

	List<SMUpdate> update = new ArrayList<SMUpdate>();
	update.add(new SMSet("home", new SMObject(geoPoint)));

	try {
	  SMObject result = ds.updateObject("user", new SMString("john"), update);
	} catch (Exception e) {}

	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMDouble.html" target="_blank
        	">SMDouble</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMSet.html" target="_blank">SMSet</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#updateObject(java.lang.String, com.stackmob.sdkapi.SMValue, java.util.List, java.util.List)" target="_blank">DataService#updateObject</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/geopoints/WriteGeo.java" target="_blank">Saving GeoPoints</a></li>
      </ul>
    </div>
  </div>
</div>



### Querying Geopoints

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
      	<li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/DataService.html#readObjects(java.lang.String, java.util.List)" target="_blank">DataService#readObjects</a></li>
      	<li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMCondition.html" target="_blank">SMCondition</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMWithin.html" target="_blank">SMWithin</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMWithinBox.html" target="_blank">SMWithinBox</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/SMNear.html" target="_blank">SMNear</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/geopoints/ReadGeo.java" target="_blank">Querying GeoPoints</a></li>
      </ul>
    </div>
  </div>
</div>


## Push Notifications

### Registering Devices

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/PushService.html#registerTokenForUser(java.lang.String, com.stackmob.sdkapi.PushService.TokenAndType)" target="_blank">PushService#registerTokenForUser</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/SMPushRegisterDevice.java" target="_blank">Register Push Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Broadcast Messages

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/PushService.html#broadcastPush(java.util.Map)" target="_blank">PushService#broadcastPush</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/BroadcastPushNotification.java" target="_blank">Broadcast Push Notification Example</a></li>
      </ul>
    </div>
  </div>
</div>

### Direct Messages

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


<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/PushService.html#sendPushToUsers(java.util.List, java.util.Map)" target="_blank">PushService#sendPushToUsers</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/push/DirectPushNotification.java" target="_blank">Send Direct Push Notifications to Users Example</a></li>
      </ul>
    </div>
  </div>
</div>


## Logging

You can write logs that you can view at <a href="https://dashboard.stackmob.com/data/logs">your Dashboard Logs</a>.

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
	LoggerService logger = serviceProvider.getLoggerService(Logging.class);

	logger.info("This is an INFO log");
	logger.error("This is ");
	logger.warn("This is ");
	logger.debug("This is ");
	
	return new ResponseToProcess(HttpURLConnection.HTTP_OK, ...);
}
```

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/Logging.java" target="_blank">Logging Example</a></li>
      </ul>
    </div>
  </div>
</div>

## External HTTP Calls

You can make calls to external APIs from custom code, but for security purposes, they must go through our `HTTPService` provider.

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

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/http/HttpService.html" target="_blank">HttpService</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/http/request/GetRequest.html" target="_blank">GetRequest</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/http/request/PostRequest.html" target="_blank">PostRequest</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/http/request/PutRequest.html" target="_blank">PutRequest</a></li>
        <li><a href="http://stackmob.github.io/stackmob-customcode-sdk/0.5.6/apidocs/index.html?com/stackmob/sdkapi/http/request/DeleteRequest.html" target="_blank">DeleteRequest</a></li>
      </ul>
    </div>
    <div class="span6">
      <strong>Examples</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/src/main/java/com/stackmob/example/util/HttpRequest.java" target="_blank">Making an External HTTP Call</a></li>
      </ul>
    </div>
  </div>
</div>

## Custom Headers and Response

You can send up custom headers in your custom code API calls.

You can also send back custom response headers and response bodies in various formats (non-JSON).

## Caching

The SDK includes functionality in `CachingService` to store key/value data in a fast, distributed cache. If your custom code does expensive computation or long running I/O, you should consider caching the results to make your code more efficient.

For example, if your code does expensive datastore queries, and the queries don't need to be fresh for each request, you could increase its performance like this (exception handling omitted for clarity):

```java
public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider provider) {
	CachingService cache = provider.getCachingService();
	String result = cache.get("expensive-query");
	if(result == null) {
		result = executeExpensiveQuery();
		cache.set("expensive-query", result, 1000); //store the result for 1000 milliseconds
	}

	return new ResponseToProcess(responseCode, ...);
}
```

A few more notes:

* The above pattern works if the datastore query can be up to 1 second stale. We recommend that you cache as much as possible and query only the parts that need to be fresh.
* We recommend holding temporary data in `CachingService` rather than memory wherever possible
* All of your app's cache data is namespaced, so it won't be overwritten by another app
* `CachingService` limits the size of each key and value, and rate limits the number of `get` and `set` calls your app can make.
* `CachingService` `get`s and `set`s are almost always faster than `DatastoreService` operations

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>API References</strong>
      <ul>
        <li><a href="http://stackmob.github.com/stackmob-customcode-sdk/0.5.6/apidocs/com/stackmob/sdkapi/caching/CachingService.html" target="_blank">CachingService</a></li>
      </ul>
    </div>
  </div>
</div>

## External Dependencies

If you're working with external libraries, you can include them with Maven or include the JAR.

### Maven

Maven helps you build your projects by also organizing your dependencies.  Many developers upload their JARs to Maven's central repository, allowing you, the developer, to simply define what resource you need in Maven's xml.  Maven will pull it in for you automatically to help build your project.

Below we include Google's `json-simple` library to help with JSON manipulation


```xml
<dependencies>
  ...
  <dependency>
    <groupId>com.googlecode.json-simple</groupId>
    <artifactId>json-simple</artifactId>
    <version>1.1.1</version>
  </dependency>
  ...
</dependencies>
```


<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-java-examples/blob/master/pom.xml" target="_blank">Example pom.xml</a></li>
      </ul>
    </div>
  </div>
</div>


## Building Custom Code

You can build your Custom Code with `Maven`, or if your project is in a GitHub repository, you can use our UI to connect StackMob with your GitHub repo and we'll build it automatically for you.

<span class="tab buildcc" title="Maven"></span>

<p><b>Using Maven to build your Custom Code JAR</b></p>

For Java, StackMob recommends using <a href="http://maven.apache.org/download.cgi" rel="nofollow" target="_blank">Maven</a>.

Our projects have `Maven`'s `pom.xml` file included, so you just have to go to the Terminal and navigate to where your `pom.xml` file is and run:

```xml
mvn package
```

This will build the JAR which you can <a href="https://dashboard.stackmob.com/module/customcode/upload" target="_blank">upload to StackMob</a>.  Your JAR can be found at:

```xml
your-project-folder-name/target/stackmob-customcode-build-0.1.0-SNAPSHOT.jar (configured in pom.xml)
```

Upon uploading, StackMob will deploy your JAR to your development environment.

All Maven files and folders are already configured for you in the <a href="#Setup">Starter project above</a>.

<div class="alert alert-info">
  <div class="row-fluid">
    <div class="span6">
      <strong>Resources</strong>
      <ul>
        <li><a href="https://github.com/stackmob/stackmob-customcode-example/tree/master/scala-maven" target="_blank" rel="nofollow">A Maven Custom Code Scala example</a></li>
      </ul>
    </div>
  </div>
</div>

<span class="tab"></span>

<span class="tab buildcc" title="GitHub"></span>

<p><b>Link StackMob with your GitHub repo</b></p>

StackMob's Custom Code service and SDK enables you to implement your own server-side logic and still have access to all the same StackMob APIs for managing your app's data. We host and run your code so you never have to worry about configuring or managing your own servers.

Using GitHub to link your Custom Code to StackMob allows you develop your code in a git repository.  StackMob can build your project f and enables one click deployment of your code for quick iteration and a much easier experience than manually building and uploading JAR files.

**Setting up GitHub with StackMob**

You can first <a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank">Link your GitHub repository with StackMob</a>.

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/customcode/modules-customcode-link-with-github-1.png" alt=""/></a></p>

Authorize access and then choose your repository:

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/customcode/modules-customcode-link-with-github-2.png" alt=""/></a></p>

Save your configuration.  Your StackMob account is now linked to GitHub!

**Deploying to Development**

Now that your repository is linked, you can build and deploy your code to the Development environment.

**Manual Deploy**

You can click a button to deploy your Custom Code to Development.

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/customcode/modules-customcode-manual-deploy.png" alt=""/></a></p>


**Enabling Automatic Build and Deploy**

You can optionally have StackMob build and deploy your latest custom code to your Development environment whenever you push changes to GitHub.  Just enable the GitHub Service Hook (tells GitHub to notify StackMob of when there are changes made).

<p class="screenshot"><a href="https://dashboard.stackmob.com/module/customcode/upload_github" target="_blank"><img src="https://s3.amazonaws.com/static.stackmob.com/images/modules/customcode/modules-customcode-service-hook.png" alt=""/></a></p>


<span class="tab"></span>



<span class="tab buildcc" title="sbt"></span>

**sbt - Simple Build Tool**

```scala
libraryDependencies += "com.stackmob" % "customcode" % "0.5.6" % "provided"
```

In our <a href="https://github.com/stackmob/stackmob-customcode-example/tree/master/scala-sbt" target="_blank" rel="nofollow">Scala custom code example</a> you'll find a sample scala-sbt project with the file <a href="https://github.com/stackmob/stackmob-customcode-example/blob/master/scala-sbt/build.sbt">build.sbt</a>. 
For those not familiar with sbt, here is the <a href="https://github.com/harrah/xsbt/wiki/Getting-Started-Setup">Getting Started with sbt</a>

<span class="tab"></span>

## Deploying to Production

To get your Custom Code to the Production environment, just <a href="https://developers.stackmob.com/module/apiversions#HowdoIDeploytoProduction" target="_blank">deploy your API</a>.  Deploying your API will roll out both schemas and custom code.

## Testing Locally

You can run Custom Code locally on a mock API server StackMob provides.  This local custom code dev environment houses your JAR and proxies API calls.  You would treat this running server as your test API server and point your requests at it.

<a href="https://blog.stackmob.com/2013/08/up-and-running-with-stackmob-custom-code-local-development/" target="_blank">Read about setting up and running your local custom code dev environment</a>.

## Best Practices

Be sure to read about <a href="https://developer.stackmob.com/customcode-sdk/best-practices-guide">Custom Code Best Practices</a>.  It'll keep your code running quickly and will help keep your code scalable!


## Restrictions

The Custom Code environment has some restrictions so as to ensure a secure environment.

* can not create new threads
* can not read/write files
* can not read/write to socket (unless using <a href="#ExternalHTTPCalls">StackMob's HTTPService</a>)
* can not read/write properties
* can not use reflection
* maximum execution time is 25 seconds
