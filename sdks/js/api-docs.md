# Javascript SDK Docs

The StackMob JS SDK connects your web application to StackMob.  You can perform CRUD operations with your objects, perform filtering queries on them, authenticate users, and more.

The StackMob JS SDK is built on <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>.

**If you are using the StackMob JS SDK from your own domain, be sure to [whitelist the domain in your app's CORS Module](https://dashboard.stackmob.com/module/cors/settings).**

** April 11, 2013: You should not use jQuery 1.9+ with the JS SDK.  Use jQuery v1.8.x until further notice. <a href="http://support.stackmob.com/entries/23492763--Invalid-JSON-returned-Error">Read about v1.9+ incompatibilities in the forum.</a>  We are currently implementing a server side fix that will resolve the issue.**

# StackMob Object

## General Setup

The StackMob JS SDK works with jQuery, Zepto, and Sencha.  To start using it, include it in your page like this. Then [initialize it](#a-init).

Note: This bundled version includes JSON2, Underscore, BackBoneJS, and the StackMob JS SDK. For unbundled, skip to the next section.

**jQuery**  

```html,4-5
<!DOCTYPE html>
<html>
  <head>
    <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
    <script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

    <script type="text/javascript">
    /* <![CDATA[ */
      StackMob.init({
        publicKey: 'your_public_key',
        apiVersion: 0
      });
    /* ]]> */
    </script>

  </head>
  <body>
    Body Content
  </body>
</html>
```

**Zepto**  

```html,1-2
<script type="text/javascript" src="http://cdnjs.cloudflare.com/ajax/libs/zepto/1.0rc1/zepto.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>
```

**Sencha**  

```html,1-2
<script type="text/javascript" src="http://cdn.sencha.com/ext-4.1.1-commercial/ext-all.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>
```

Using the method above, everything you need (except for jQuery, Sencha, or Zepto) is bundled into one JS resource for 
you (v0.6.0+).  However, if you prefer to separate out the dependencies from the StackMob JS SDK, you can do that like this. Notice the 
string `bundled` missing from the stackmob resource.

```javascript,5-8

<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/json2-min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/underscore-1.4.4-min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/backbone-1.0.0-min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
  StackMob.init({
    publicKey:        'your_public_key',
    apiVersion:       0
  });
/* ]]> */
</script>

</head>
<body>
  Body Content
</body>
</html>

```

## init

Call `init` to initialize the StackMob library.  This is required before you use StackMob's JS SDK further.

You can find your public key on your <a href="https://dashboard.stackmob.com/settings" target="_blank">App Info page</a>. Note that with version 0.9.0+, you no longer need to specify an appName or clientSubdomain.

**JS SDK Version 0.9.0+**

```javascript,2-5
<script type="text/javascript">
StackMob.init({
  publicKey:        'your_public_key',
  apiVersion:       0
});
</script>
```

Optional Parameters:

```javascript,5-7
<script type="text/javascript">
StackMob.init({
  publicKey:        'your_public_key',
  apiVersion:       0,
  // Optional Advanced Parameters. Details below.
  useRelativePathForAjax: true
  // See additional properties below.
});
</script>
```

<table class="table table-bordered table-pretty-header">
  <thead>
    <tr>
      <th>Property</th>
      <th>Value</th>
      <th>Description</th>
      <th>Default</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td class="keycell">useRelativePathForAjax</td>
      <td>Boolean</td>
      <td>
        <p>
          Only set this to <code>true</code> if you are using a <a href="https://marketplace.stackmob.com/module/customdomains">StackMob Custom Domain</a>. It will increase browser compatibility by making API requests without using CORS.  Apps hosted by the <a href="https://marketplace.stackmob.com/module/html5">HTML5 Module</a> automatically enable this option.
        </p>
      </td>
      <td>For apps hosted on StackMob HTML5 Hosting, <code>true</code>.  Otherwise <code>false</code>.</td>
    </tr>
    <tr>
      <td class="keycell" rowspan="3">secure</td>
      <td><code>StackMob.SECURE_ALWAYS</code></td>
      <td>
          All requests will be made over HTTPS
        </div>
      </td>
      <td rowspan="3">
        <code>StackMob.SECURE_ALWAYS</code> from URLs beginning with <b>https://</b>. 
        <code>StackMob.SECURE_NEVER</code> from URLs beginning with <b>http://</b>. 
        <code>StackMob.SECURE_MIXED</code> otherwise.</td>
    </tr>
    <tr>
      <!-- filled -->
      <td><code>StackMob.SECURE_NEVER</code</td>
      <td>All requests will be made over HTTP</td>
    </tr>
    <tr>
      <!-- filled -->
      <td><code>StackMob.SECURE_MIXED</code></td>
      <td>Only authentication methods and user creation will be made over HTTPS</td>
    </tr>
    <tr>
      <td class="keycell">apiURL <br/><span class="deprecated">Deprecated v0.9.0</span></td>
      <td>URL</td>
      <td>If you set this parameter to <code>http://api.stackmob.com</code> in the past, such as for PhoneGap, you should no longer set this field. Since SDK v0.8.0, the default API Domain is <code>api.stackmob.com</code>.  HTTPS requests will be determined by the <code>secure</code> parameter.</td>
      <td><code>http://api.stackmob.com</code></td>
    </tr>
    <tr>
      <td class="keycell">userSchema <span class="deprecated">Deprecated v0.9.0</span></td>
      <td>String</td>
      <td>If you have a different User schema than the default "user" schema. See <a href="#a-login_-_with_custom_schema">login with custom user schema</a>.</td>
      <td><code>user</code></td>
    </tr>
    <tr>
      <td class="keycell">loginField <span class="deprecated">Deprecated v0.9.0</span></td>
      <td>String</td>
      <td>Specify custom username field. See <a href="#a-login_-_with_custom_schema">login with custom user schema</a>.</td>
      <td><code>username</code></td>
    </tr>
    <tr>
      <td class="keycell">passwordField <span class="deprecated">Deprecated v0.9.0</span></td>
      <td>String</td>
      <td>Specify custom password field. See <a href="#a-login_-_with_custom_schema">login with custom user schema</a>.</td>
      <td><code>password</code></td>
    </tr>
  </tbody>
</table>

**JS SDK Version 0.8.0**

```javascript,2-7
<script type="text/javascript">
StackMob.init({
  appName:          'your_app_name',
  clientSubdomain:  'your_client_subdomain',
  publicKey:        'your_public_key',
  apiVersion:       0
});
</script>
```

**JS SDK Version 0.1.1+**

```javascript
<script type="text/javascript">
StackMob.init({
	appName:         'your_app_name',
	clientSubdomain: 'your_client_subdomain',
  apiVersion:      0
});
</script>
```

**JS SDK Version 0.1.0**

```javascript
<script type="text/javascript">
StackMob.init({
  appName: 'your app name',
  dev: true
});
</script>
```

## customcode

**Introduced:** JS SDK Version 0.1.0

**Updated Signature:** JS SDK Version 0.6.0

**Description:**

```javascript
customcode(customcodeName, customcodeParameters, method, options)

customcodeName - the name of your custom code method
customcodeParameters - a hash of parameters you are passing to your custom code
method - either 'GET', 'POST', 'PUT', or 'DELETE'. You can call customcode without method param: customcode(customcodeName, customcodeParameters, options) and the method param will be set to default ('GET').
options - a hash containing, but not limited to "success" and "error" callbacks

```

StackMob gives you the ability to write and upload server side code that you can execute from the client side.  This uses our <a href="https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK" target="_blank">Custom Code SDK</a>.

e.g. If you write a Java/Scala method named `getloggedinuser` and upload it to StackMob, you can call that server-side code from your client-side JS SDK and return value of your Java/Scala method.

**Usage:**

```javascript,33-46,52-65,71-84,90-103

<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
  StackMob.init({
		publicKey: 'your_public_key',
		apiVersion: 0
  });
/* ]]> */
</script>

</head>
<body>


<script type="text/javascript">
/* <![CDATA[ */
  /*
   The following example assumes you've uploaded custom code to StackMob's servers, running your code in a JVM server-side. 
   It assumes you have a method named "helloworld" that can take a parameter "who"
   
   The server-side code simply returns "Hello World, Kodi!"

   This is for GET. You can process the data passed to your custom code by looking at this tutorial:
   http://developer.stackmob.com/tutorials/customcode/Fetching-Parameters-JSON-and-non-JSON
   */
  StackMob.customcode('helloworld', 
  	{ who: "Kodi" }, //method parameters
	'GET', //skip third param for default 'GET'
  	{
  		success: function(result) {
  			console.debug(result); //prints out the returned JSON your custom code specifies
  			/*
  			 {
  			 	msg: "Hello World, Kodi!"
  			 }
  			 */
  		}
  	} 
  );

  /*
   This is for DELETE. You can process the data passed to your custom code by looking at this tutorial:
   http://developer.stackmob.com/tutorials/customcode/Fetching-Parameters-JSON-and-non-JSON
   */
  StackMob.customcode('helloworld', 
  	{ who: "Kodi" }, //method parameters
	'DELETE', //skip third param for default 'GET'
  	{
  		success: function(result) {
  			console.debug(result); //prints out the returned JSON your custom code specifies
  			/*
  			 {
  			 	msg: "Hello World, Kodi!"
  			 }
  			 */
  		}
  	} 
  );

  /*
   This is for POST. You can process the data passed to your custom code by looking at this tutorial:
   http://developer.stackmob.com/tutorials/customcode/Fetching-Parameters-JSON-and-non-JSON
   */
  StackMob.customcode('helloworld', 
  	{ who: "Kodi" }, //method parameters
	'POST', //skip third param for default 'GET'
  	{
  		success: function(result) {
  			console.debug(result); //prints out the returned JSON your custom code specifies
  			/*
  			 {
  			 	msg: "Hello World, Kodi!"
  			 }
  			 */
  		}
  	} 
  );

  /*
   This is for PUT. You can process the data passed to your custom code by looking at this tutorial:
   http://developer.stackmob.com/tutorials/customcode/Fetching-Parameters-JSON-and-non-JSON
   */
  StackMob.customcode('helloworld', 
  	{ who: "Kodi" }, //method parameters
	'PUT', //skip third param for default 'GET'
  	{
  		success: function(result) {
  			console.debug(result); //prints out the returned JSON your custom code specifies
  			/*
  			 {
  			 	msg: "Hello World, Kodi!"
  			 }
  			 */
  		}
  	} 
  );
/* ]]> */
</script>

</body>
</html>

```

**Automatic Retry:**  

Introduced: JS SDK Version 0.6.0  

If the server returns an HTTP Status of 503 (Service Unavailable), the JS SDK will automatically retry the same command `StackMob.RETRY_ATTEMPTS` (default of 3) times with an interval of `StackMob.RETRY_WAIT` (default of 10000) milliseconds.  If the server responds with a `retry-after` header, that value will be used instead of `StackMob.RETRY_WAIT`.  This is particularly useful for custom code, when your server instance may take a few seconds to initialize after being idle for some period.


## cc

**Introduced:** JS SDK Version 0.1.0

A shorthand method name for `customcode`.  See `customcode` above.

## isLoggedIn - StackMob

```javascript
StackMob.isLoggedIn( options )

options - object containing `yes` and `no` functions to be executed as callbacks
```

**Introduced:** JS SDK Version 0.7.0

**Description:**

Asynchronous method to check if any user is logged in. Refresh tokens will be renewed if necessary.  
Will execute `options.yes(username)` if logged in.  
Will execute `options.no()` if not logged in.  

**Usage:**

```javascript,6-13,17
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.isLoggedIn({
    yes: function(username){
      console.log(username + " is logged in.");
    },
    no: function(){
      console.log("No one is currently logged in.");
    }
  });
```

<h3>isLoggedIn - Synchronous (Deprecated)</h3>

```javascript
StackMob.isLoggedIn()

returns boolean - true if a user is currently logged in, false otherwise
```
**Introduced:** JS SDK Version 0.1.1

**Description:**
Check to see if any user is currently logged in.

This version of StackMob.isLoggedIn() is being deprecated because it will not check the server for an extended session; potentially reporting that a user is not logged in even if though their session can be extended.

**Usage:**

```javascript,6
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.isLoggedIn(); //returns true
```

## isUserLoggedIn

```javascript
StackMob.isUserLoggedIn(username, options)

username - the username of the user you want check
options - object containing 'yes' and 'no' functions to execute as callbacks
```

**Introduced:** JS SDK Version 0.7.0

**Description:**

Asynchronous method to check if a particular user is logged in.  Refresh tokens will be renewed if necessary.  
Will execute `options.yes(username)` if `username` is logged in.  
Will execute `options.no()` if `username` is not logged in.  

**Usage:**

```javascript,6-13,15-22
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.isUserLoggedIn('Chuck Norris', {
    yes: function(username){
      console.log(username + " is logged in."); // Outputs 'Chuck Norris is logged in.'
    },
    no: function(result){
      console.log(username + " is logged in.");
    }
  });

  StackMob.isUserLoggedIn('Marty McFly' {
    yes: function(username){
      console.log(username + " is logged in.");
    },
    no: function(result){
      console.log(username + " is logged in."); // Outputs 'Chuck Norris is logged in.'
    }
  });
```

<h3>isUserLoggedIn - Synchronous (Deprecated)</h3>

```javascript
StackMob.isUserLoggedIn(username)

username - the username of the user you want check
```
**Introduced:** JS SDK Version 0.1.1

**Description:**

Check to see if a particular user is logged in.  This version of isUserLoggedIn is being deprecated because it will not check the server for an extended session; potentially reporting that a user is not logged in even if though their session can be extended.

**Usage:**

```javascript,6-13,15-22
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.isUserLoggedIn('Chuck Norris'); //returns true
  StackMob.isUserLoggedIn('Marty McFly'); //returns false
```

## getLoggedInUser

```javascript
StackMob.getLoggedInUser(options)

options - object containing 'success' function to execute as a callback
  success - function(username)
  username - username of the currently logged in user.  null if no user is logged in.
```
**Introduced:** JS SDK Version 0.7.0

**Description:**

Asynchronous method to retrieve the username of the logged in user. Refresh tokens will be renewed if necessary.  
Will execute `options.success(username)` on completion.  

**Usage:**

```javascript,7-11
  //Declare this once on the page
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.getLoggedInUser( {
    success: function(username){
      console.log(username + " is logged in.");  // Outputs 'Chuck Norris is logged in.'
    }
  });
```

<h3>getLoggedInUser - Synchronous (Deprecated)</h3>

```javascript
StackMob.getLoggedInUser()

returns the username of the currently logged in user.  null if no user is logged in.
```
**Introduced:** JS SDK Version 0.1.1

**Description:**

Get the username of the currently logged in user.

This version of getLoggedInUser is being deprecated because it will not check the server for an extended session; potentially reporting that a user is not logged in even if though their session can be extended.

**Usage:**

```javascript,7
  //Declare this once on the page
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  StackMob.getLoggedInUser(); //returns 'Chuck Norris'
```

## getOAuthExpireTime

**Introduced:** JS SDK Version 0.5.0

**Description:**

```javascript
getOAuthExpireTime()

Returns a long.  Time in milliseconds for when the current OAuth credentials expire.
```

Returns the date in milliseconds for when the current OAuth 2.0 credentials expire.


**Usage:**

```javascript
//Assuming a user is logged in..

var expireDate = new Date(StackMob.getOAuthExpireTime());

console.debug('The OAuth 2.0 credentials expire(d): ' + expireDate);

```

# Callbacks and Options

## Success Callback

**Introduced:** JS SDK Version 0.1.0

**Description:**

The `success` callback is called after an operation has successfully completed an asynchronous command.

`options` is new as of JS SDK v0.9.0.

```javascript
function(model, result, options)

model - The model that initiated the call
result - The response returned from the StackMob server; typically JSON
options - the options passed into the original call
```

**Usage:**

Used in the options `optionshash` params of various methods.  Called if there's a successful AJAX call.  Use it to handle
follow up operations after the asynchronous call is finished.

```javascript
var user = new User({ username: 'Frankenstein', password: 'muahaha' });
user.create({
  success: function(model, result, options) {
    console.debug(model.toJSON());
  }
});
```

## Error Callback

**Introduced:** JS SDK Version 0.1.0

**Description:**

The `error` callback is called after an operation has unsuccessfully completed an asynchronous command.

```javascript
function(model, response, options)

model - The model that initiated the call
response - The JSON response/error returned from the StackMob server
options - the options passed into the original call
```

**Usage:**

Used in the options `optionshash` params of various methods.  Called if there's a failure in the AJAX call.  Use it to handle follow-up operations after the asynchronous call fails.

```javascript
var user = new User({ username: 'duplicateuser', password: 'youagain?' });
user.create({
  error: function(model, response, options) {
    //argh, failed to take over the world again!
    console.debug(response);
    /*
      Output:

      {
        "error": "Duplicate primary key encountered"
      }
     */

  }
});
```

## Complete Callback

**Introduced:** JS SDK Version 0.6.0

**Description:**  

The `complete` callback is called after an asynchronous command has finished executing, regardless of its success.  All methods on the StackMob JS SDK accept a success callback via the options hash.

```javascript
function(model, response)

model - The model that initiated the call
response - The JSON response returned from the StackMob server
```

**Usage:**  

Used in the options `optionshash` params of various methods.  Called on completion of an AJAX call.  Use it to handle-follow up operations after an asynchronous call completes.

```javascript
var user = new User({ username: 'maybeDuplicateUser', password: 'uncertainty'});
user.create({
  complete: function(model, response) {
      consolge.debug("I tried, but I'm unsure if I succeeded.");
  }
});
```

## Secure Option

**Introduced:** JS SDK Version 0.9.0

**Description:**  

The `secureRequest` parameter can be set on any method to force its request to be sent over HTTPS.

```javascript,8
    var Book = StackMob.Model.extend({ schemaName: 'book' });

    var book = new Book();
    book.create({
      success: function(){
        // ...
      },
      secureRequest: true
    });
```

# StackMob.Model

**Introduced:** JS SDK Version 0.1.0

**Description:**

Simply extend `StackMob.Model` to define your class.  StackMob.Model is an extension of the <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">Backbone.js Model</a>, so you can use any method that a <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">Backbone.js Model</a> provides as well.


```javascript
var Book = StackMob.Model.extend({
  schemaName: 'book' //this maps a schema at https://dashboard.stackmob.com/schemas/
});
```

The `schemaName` should match the schema name you want on the <a href="https://dashboard.stackmob.com/schemas/" target="_blank">StackMob schemas page</a>.

<b><a name="declare_binary_fields">Binary Fields</a></b>

If you have `binary fields`, declare them so that the JS SDK can handle them properly.

```js,3
var Book = StackMob.Model.extend({
  schemaName: 'book', //this maps a schema at https://dashboard.stackmob.com/schemas/
  binaryFields: ['frontcoverpic', 'backcoverpic']
});
```

**Usage:**

```javascript
  //Declare this once on the page
  var Book = StackMob.Model.extend({
    schemaName: 'book'
  });

  var a_book = new Book({ 'title': 'The Complete Calvin and Hobbes', 'author': 'Bill Watterson' });
  var another_book = new Book({ 'title': 'Deep Thoughts', 'author': 'Jack Handey' });

  //get(...) is a Backbone.js Model method
  console.debug(a_book.get('title')); //prints out "The Complete Calvin and Hobbes"
  console.debug(another_book.get('author')); //prints out "Jack Handey"
```

## create

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
create(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks.
```

Use this to create new instances of an object in your datastore.  If you do not provide a primary key, StackMob will generate and return one for you.

The `User` class's primary key is `username`.  Classes that you define will have a primary key of `[your schema name]_id'.  e.g. `book_id`


**Usage:**

```javascript

  var user = new StackMob.User({ username: 'toucansam', password: 'fruitloops', age: 15, favoriteflavors: ["lemon", "blueberry", "prime rib"] });
  user.create({
    success: function(model, result, options) {
      //jackpot! fruity breakfasts for everyone!
    },
    error: function(model, result, options) {
      //failed to create the cereal of my dreams
    }
  });
</script>

</body>
</html>

```


## save

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
save(jsonToUpdate, optionsHash)

jsonToUpdate - a partial JSON object containing the fields you want to edit
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks.
```

Call `save` to update an existing object by specifying the primary key and the fields you want to modify.

**Usage:**

```javascript,2,7-10
  var user = new StackMob.User({ username: 'toucansam', age: 20, brand: 'Kelloggs' });
  user.save(); //update toucansam's age and add a "brand"

  //or...

  var user1 = new StackMob.User({ username: 'capncrunch' });
  user1.save({ age: 51, favoritefood: 'cereal' }, { //update age and add 'favoritefood' field
    success: function(model, result, options) {},
    error: function(model, result, options) {},
  });
```

**Additional options:**

`remote_ignore`

*Default false*  

**Introduced:** JS SDK Version 0.6.0  

**Description:**  
If you have fields on an object that you do not want to persist to StackMob, you can specify a list of fieldnames to ignore with the `remote_ignore` property on `optionsHash`.

**Usage:**  

```javascript
  var user = new StackMob.User({ username: 'toucansam', age: 20, brand: 'Kelloggs' });

  // Add a field to the user object that we don't want to save to the StackMob server
  user.set('ignoreMe', "secret local value");

  // Let's make sure we don't submit the `ignoreMe` to the server!
  user.save({ age: 51, favoritefood: 'cereal' }, {
    success: function(model) {},
    error: function(model, response) {},
    "remote_ignore": ["ignoreMe"]
  });
```


## fetch

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
fetch(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks.
```

Need to read an object or update an existing local copy?  Specify the primary key and do a `fetch`.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'toucansam' });
  user.fetch(); //fetches the full user 'toucansam' asynchronously from your StackMob datastore
```

## fetchExpanded

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
fetchExpanded(depth, optionsHash)

depth - Number between 0-3 (higher numbers will be ignored)
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

If you have <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationships</a> defined on your object, the relations are kept by references to the primary key(s).  Fetching an expanded version of your object at a depth of "1" will return those relationships as the full object, not just a reference to the primary key(s).

Because those related objects themselves may have relationships, you can expand those by expanding to a depth of "2".  Read more about <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships#a-expanding_relationships:_the__expand_parameter" target="_blank">Relationships and Expand</a>.


**Usage:**

```javascript
  var user = new StackMob.User({ username: "Mr. Lemming" });
  user.fetchExpanded(1);
```

## query - StackMob.Model

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
query(stackmobModelQuery, optionsHash)

stackmobModelQuery - a StackMob.Model.Query representing your query
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Fetches a result using the provided query parameters.

**Usage:**

```javascript
var q = new StackMob.Model.Query();
q.select('age');

var user = new StackMob.User({ username: 'Chuck Norris' });
user.query(q);

//...

/*
 Returns
 {
 	'username': 'Chuck Norris',
 	'age': 50
 }
 */
```

## destroy

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
destroy(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks.
```

Delete an object from the server with `destroy`.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'toucansam'});
  user.destroy(); //deletes "toucansam" from the server
```



## get

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
get(fieldName)

fieldName - The fieldname you want from your object.
```

Get the value of one of your object's fields.

This is a Model method from <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>.  StackMob Models have all methods Backbone Models have. Find <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">additional methods about Backbone.js Models</a>


**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.get('username'); //returns "Chuck Norris"
```

## set

**Introduced:** JS SDK Version 0.1.0

**Description:**


```javascript
set(fieldValueHash)

fieldValueHash - the hash object that you want to set
```

Sets the value of your object for the given fieldName.

This is a Model method from <a href="http://documentcloud.github.com/backbone/" target="_blank">Backbone.js</a>.  StackMob Models have all methods Backbone Models have. Find <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">additional methods available to Backbone.js Models</a>


**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.set({profession: 'taker of names' });
  console.debug(user.toJSON());
  /*
   * prints out:
   * {
   *   username: 'Chuck Norris',
   *   password: 'myfists',
   *   profession: 'taker of names'
   * }
   *
   *
   */
```

## setBinaryFile

**Note!!!** Be sure to <a href="#declare_binary_fields">declare your binary fields</a> in your `StackMob.Model`!

**Introduced:** JS SDK Version 0.2.1

**Description:**


```javascript
setBinaryFile(fieldName, filename, filetype, base64EncodedData)

fieldName - a string representing the name of the field in your schema that will contain the binary file (e.g. profile_pic).
filename - a string representing the name of the file being uploaded.
filetype - a string representing the file type of the file being uploaded (e.g. "image/jpeg" or "image/png").
base64EncodedData - a string representing the base64-encoded file contents.

```

Works with a `binary field` type. Call this method instead of "set()" whenever you need to set the value of a binary field.

The example below walks you through not only how to set a base64 encoded file, but how to actually retrieve such a file from disk.
This example utilizes File and FileReader objects, available via HTML5 - but an HTML5 feature currently not supported by Safari.

**Usage:**

```javascript
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
	StackMob.init({
		publicKey: 'your_public_key',
  	apiVersion: 0
  });
/* ]]> */
</script>

</head>
<body>

<table>
  <tr>
    <td>File to Encode:</td>
    <td><input type="file" id="files" name="files[]" multiple /></td>
  </tr>
</table>

<script type="text/javascript">
  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var todoInstance = new Todo();
  var fieldname = 'picture'; //name of binary field that you created in your schema
 
  function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object

    // Loop through the FileList
    for (var i = 0, f; f = files[i]; i++) {

      var reader = new FileReader();

      // Closure to capture the file information
      reader.onload = (function(theFile) {
        return function(e) {

          /*
            e.target.result will return "data:image/jpeg;base64,[base64 encoded data]...".
            We only want the "[base64 encoded data] portion, so strip out the first part
          */
          var base64Content = e.target.result.substring(e.target.result.indexOf(',') + 1, e.target.result.length);
          var fileName = theFile.name;
          var fileType = theFile.type;

          todoInstance.setBinaryFile(fieldname, fileName, fileType, base64Content);
          todoInstance.save();

        };
      })(f);

      // Read in the file as a data URL
      fileContent = reader.readAsDataURL(f);

    }
  }

  document.getElementById('files').addEventListener('change', handleFileSelect, false);

</script>

</body>
</html>
```

## incrementOnSave

**Introduced:** JS SDK Version 0.3.0

**Description:**


```javascript
incrementOnSave(fieldName, value)

fieldName - a string representing the name of the counter/numeric fieid in your schema that will incremented.
value - the numeric value you want the field incremented by. Note that positive integers are expected, but negative
integers can be provided in order to decrement the counter.  However, there is a separate decrementOnSave() method
that can be used for this process.

```

Intended for use with a numeric field that will be used as a counter. Calling this method will ensure that the current value of the counter will be incremented by the value provided,
even if another call updates the value before this call can execute.  Value will be updated once a call to save() is made.

The example below walks you through how to increment a field, and how to make sure that increment gets executed.

**Usage:**

```javascript
  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  // We need to provide an id of the row containing the counter we want updated
  var todoInstance = new Todo({ todo_id : 'testid' });
  todoInstance.incrementOnSave('mycount', 2); // the field "mycount" will be incremented by 2
  todoInstance.save(); // Call save() to make sure the increment gets executed
```

## decrementOnSave


```javascript
decrementOnSave(fieldName, value)

fieldName - a string representing the name of the counter/numeric fieid in your schema that will decremented.
value - the numeric value you want the field decremented by.  This should be a postive integer (if a negative integer is provdided, the counter will be incremented by that value instead).

```

Intended for use with a numeric field that will be used as a counter. Calling this method will ensure that the current value of the counter will be decremented by the value provided,
even if another call updates the value before this call can execute.  Value will be updated once a call to save() is made.

The example below walks you through how to decrement a field, and how to make sure that decrement gets executed.

**Usage:**

```javascript
  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  // We need to provide an id of the row containing the counter we want updated
  var todoInstance = new Todo({ todo_id : 'testid' });
  todoInstance.decrementOnSave('mycount', 2);  // the "mycount" field will be decremented by 2
  todoInstance.save(); // Call save() to make sure the decrement gets executed
```


## appendAndCreate

**Introduced:** JS SDK Version 0.1.0

** Deprecated:** JS SDK Version 0.1.1

This method was introduced in JS SDK version 0.1.0 but has been deprecated in 0.1.1.  Use <a href="#a-addrelationship">addRelationship</a> instead.

## addRelationship

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
addRelationship(fieldName, items, optionsHash)

fieldName - The relationship array field you want to append items to
items - Array of new objects you want to save to StackMob and simultaneously add to the existing array of fieldName
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

For use with <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationships</a>.  You can use "addRelationship" to create objects and then relate them to a parent object in one call.  

This also ensures that if more than one request appends to this field at a time, that the results will be appended correctly and no requests will be overwritten.

**Usage:**

Assume that we have a Todo application, where a User has a one to many relationship of Todo items.  Presume the following example JSON to represent the User and Todo schemas respectively.

```javascript
// User Schema/Object
{
  username: 'Brain',
  password: 'andPinky',
  todos: ['12345', '6789'], //relationship field: one to many Todo objects.  Array of related Todo object IDs
  ...
}

//Todo Schema/Object
{
  todo_id: '12345',
  action: 'take over the world.. again',
  ...
}
```

And the code:

```javascript
  // Define once
  var Todo = StackMob.Model.extend({
    schemaName: 'todo'
  });

  var Todos = StackMob.Collection.extend({
    model: Todo
  });


  // Prepare new todo items locally.  We want to:
  // 1. create these on StackMob
  // 2. append them as related objects to the user
  // ... both in one API call
  var todos = [
      new Todo({ action: 'take over the world, Tuesday' }),
      new Todo({ action: 'take over the world, Wednesday' }),
      new Todo({ action: 'take over the world, Thursday' })
    ];

  var user = new StackMob.User({ username: 'Brain' });

  //append the related objects and create them on StackMob
  user.addRelationship('todos', todos, {
    success: function(model, result, options) {
      //Now that StackMob's done saving the new Todos and updated the User's field,
      //Let's view the todos.
      var alltodos = new Todos();
      alltodos.fetch({
        success: function(collection, result, options) {
          console.debug(collection.toJSON()); //You'll find the todos are now saved on StackMob.
          /*
            Output:
            [
              { todo_id: '12345', action: 'haircut', lastmoddate: ..., createddate: ... },
              { todo_id: '6789', action: "Remind Pinky what we're doing tonight", ... },
              { todo_id: '1', action: 'take over the world, Tuesday', ... },
              { todo_id: '2', action: 'take over the world, Wednesday', ... },
              { todo_id: '3', action: 'take over the world, Thursday',  ... },
            ]
           */
        }
      });

      console.debug(model.toJSON()); //Meanwhile, the user returned from StackMob will have the Todo items as related objects now.
      /*
        Output:
        {
          username: 'Brain',
          todos: ['12345', '6789', '1', '2', '3'],
          ...
        }
       */
    }
  });
```


## appendAndSave

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
appendAndSave(fieldName, values, optionsHash)

fieldName - The array field you want to append items to
values - Array of values or IDs of existing objects you want to add to the existing value of fieldName
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

For use with arrays (including <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationship</a> arrays).

Arrays: Append values onto the array at `fieldName`.
Relationship Arrays:  Append IDs of existing objects to the array at `fieldName`

This also ensures that if more than one request appends to this field at a time, that the results will be appended correctly and no requests will be overwritten.

**Usage:**


```javascript,32,33,34,43,44
  /*
    Assume Snoopy is an existing user on StackMob:
    {
      username: 'Snoopy',
      favoritecolors: ['black', 'white'],
      ...
    }
   */
  var user = new StackMob.User({ username: 'Snoopy' });
  user.appendAndSave('favoritecolors', ['gray','dark gray'], {
    success: function(model, result, options) {
      console.debug(model.toJSON());
      /*
        Output:
        {
          username: 'Snoopy',
          favoritecolors: ['black','white','gray','dark gray'],
          ...
        }
       */
    }
  });
```

## deleteAndSave

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
deleteAndSave(fieldName, values, cascadeDelete, optionsHash)

fieldName - The array field from which you want to delete values
values - The values in the array you want to delete
cascadeDelete - If the array represents a relationship, setting this to true will also delete the
                instance of the object in the datastore.  If this is false, this will only delete
                the reference to that instance.
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

For use with arrays (including <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationship</a> arrays).

Arrays: Delete values from the array at `fieldName`.
Relationship Arrays:  Delete IDs of existing objects from the array at `fieldName`.  If `cascadeDelete` is true, then also delete the instances of the objects from their respective datastores as well. (soft vs. hard delete)

**Usage:**

```javascript,35,36,37,46,47
  /*
    Assume Snoopy is an existing user on StackMob:
    {
      username: 'Snoopy',
      favoritecolors: ['black', 'white', 'gray', 'dark gray'],
      ...
    }
   */
  var user = new StackMob.User({ username: 'Snoopy' });
  user.deleteAndSave('favoritecolors', ['gray','dark gray'], StackMob.SOFT_DELETE, {
    success: function(model, result, options) {
      console.debug(model.toJSON());
      /*
        Output:
        {
          username: 'Snoopy',
          favoritecolors: ['black','white'],
          ...
        }
       */
    }
  });
```



## Backbone.Model Methods

StackMob.Model extends Backbone.Model and hence inherits all of its methods.

See the <a href="http://documentcloud.github.com/backbone/#Model" target="_blank">API Docs for Backbone.Model</a>.


# StackMob.User

**Introduced:** JS SDK Version 0.1.0

**Description:**

The `StackMob.User` object is a special StackMob object that extends the `StackMob.Model` and hence has all methods available to it from `StackMob.Model` and `Model` from Backbone.js.

**Usage:**

```javascript
//Create a user on StackMob
var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists', weaponofchoice: 'nunchucks' });
user.create();
```

## login

**Introduced:** JS SDK Version 0.1.0

**Upgraded to OAuth 2.0:** JS SDK Version 0.5.0

**Introduced refresh tokens:** JS SDK Version 0.6.0

**Description:**

```javascript
login(keepLoggedIn, optionsHash)

keepLoggedIn - Tell the sdk whether or not to use refresh tokens and keep the user logged in
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

**JS SDK v0.5.x+**

Authenticate a `StackMob.User`.  When StackMob verifies that the username and password match that of the user in the datastore, StackMob returns OAuth 2.0 credentials to the client.  The JS SDK stores the OAuth 2.0 credentials and use them to sign the user's subsequent API requests with one-time use encrypted tokens.

Logging in with OAuth 2.0 gives you access to <a href="https://www.stackmob.com/devcenter/docs/Access-Controls:-Schema-Permissions" target="_blank">Access Controls</a>, where you can control who can perform what actions on your schemas on a user-by-user basis.

** JS SDK v0.4.0-**

Authenticate a `StackMob.User`.  When StackMob verifies that the username and password match that of the user in the datastore, a session is created on the server side and a cookie is sent back to your client.  Example cookie: `session_2562=_qK3NSDna1PcfNm_pmA...`.  Subsequent requests to the server will include the cookie in the request headers to help identify the identity of your user in follow up requests.

**Usage:**

```js
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login(false, {
    success: function(model) {},
    error: function(model, response) {}
  });
```



**Additional Configuration (Optional)**

`fullyPopulateUser (Boolean)`

*Default false*

If true, fully populate the user model from the server on-login.  This will overwrite your local user fields.  This is useful if you're logging the user in for the first time.

```js
var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
user.login(true, {
	success: function(model, result, options) {},
	error: function(model, result, options) {},
	fullyPopulateUser: true
});
```

Results in a populated user model onlogin.  `user.toJSON()` results in:

```js
//post-login: user.toJSON() with fullyPopulateUser 
{
  username: 'Chuck Norris',
  createddate: '123...',
  lastmoddate: '123...',
  age: 50,
  hobbies: [],
  ...
}
```

Otherwise the result would be:

```js
//post-login: user.toJSON() without fullyPopulateUser
{
	username: 'Chuck Norris',
	...
}
```

## login - with custom schema

You don't have to use the existing `user` schema for user actions.  To use your own schema, make sure you select 'Create as a User Object' when creating a new schema.  In JavaScript, define a new user class extending from `StackMob.User` specifying your custom fields.

```javascript
		var CustomUser = StackMob.User.extend({
			schemaName: 'customuser'
			loginField: 'customusername',
			passwordField: 'custompassword'
		});
```

Then use the new class for your user objects:

```js
    var user = new CustomUser({ customusername: 'Chuck Norris', custompassword: 'myfists' });
    user.login(false, {
      success: function(model, result, options) {},
      error: function(model, result, options) {}
    });
```

## logout

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
logout(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Logout a user.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris'});
  user.logout();
```

## isLoggedIn - StackMob.User

```javascript
isLoggedIn( options )

options - object containing `yes` and `no` functions to be executed as callbacks
```
**Introduced:** JS SDK Version 0.7.0

**Description:**

Asynchronous method to check if this user is logged in.  
Will execute `options.yes(username)` if `user` is logged in.  
Will execute `options.no()` if `user` is not logged in.  

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  user.isLoggedIn({
    yes: function(){
      console.log("Logged in.");
    },
    no: function(){
      console.log("Not logged in.");
    }
  });
```

<h3>isLoggedIn - Synchronous (Deprecated)</h3>

```javascript
isLoggedIn()

returns true if the user is logged in
```
**Introduced:** JS SDK Version 0.1.1

**Description:**

Check to see if this user is logged in.

This version of isLoggedIn() is being deprecated because it will not check the server for an extended session; potentially reporting that a user is not logged in even if though their session can be extended.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  user.isLoggedIn(); //returns true
```

## forgotPassword

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
forgotPassword(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Call this to have StackMob email a temporary password to the user's email address (as specified in the <a href="https://dashboard.stackmob.com/schemas/edit/user" target="_blank">User schema</a>). See the <a href="https://www.stackmob.com/devcenter/docs/User-Authentication-API#a-forgot_password" target="_blank">Forgot Password</a> feature.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris' });
  //assume StackMob.User has an "email" field, and Chuck Norris's email is "chucknorris@somewhere.com"
  user.forgotPassword(); //tells StackMob to send an email to Chuck Norris at "chucknorris@somewhere.com"
```

## loginWithTempAndSetNewPassword

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
loginWithTempAndSetNewPassword(tempPassword, newPassword, keepLoggedIn, optionsHash)

tempPassword - the temporary password that was emailed to this user
newPassword - a new user-provided password to replace the temporary one
keepLoggedIn - Tell the sdk whether or not to use refresh tokens and keep the user logged in
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

After a user has received a temporary password, the user can login with their temporary password but must also supply a new user-provided password to replace it. Use this to do so.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris' });
  user.forgotPassword(); 

  //after checking getting his temp password in his email...

  user.loginWithTempAndSetNewPassword('temporary password from StackMob email', 'mysuperfists');

  //...

  StackMob.getLoggedInUser(); //returns 'Chuck Norris'

  //StackMob will log the user in with the temporary password and then give the user a new password: 'mysuperfists' for future logins
```

## resetPassword

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
resetPassword(oldPassword, newPassword, optionsHash)

oldPassword - the user's old password
newPassword - the user's new password
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

After a user has logged in, he/she can change his/her password via `resetPassword`.  Again, the user **must be logged in** in order ot use this method.

**Usage:**

```javascript
  var user = new StackMob.User({ username: 'Chuck Norris', password: 'myfists' });
  user.login();

  //...

  user.resetPassword('myfists', 'mynewandimprovedfists'); //changes password from 'myfists' to 'mynewandimprovedfists'
```

## createUserWithFacebook

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
createUserWithFacebook(facebookAccessToken, options)

facebookAccessToken - the user's Facebook Access Token after the user logs into Facebook with the Facebook JS SDK
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Create a StackMob user instance and associate it with the user's Facebook account.  This way the user can log into your app in the future with Facebook.

**Usage:**

```javascript
<html xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
	StackMob.init({
		publicKey: 'your_public_key',
  	apiVersion: 0
  });
/* ]]> */
</script>

</head>
<body>
<script>
  //What follows is standard Facebook JS initialization.  See Facebook for more details.
  //Facebook JS SDK code here.  Use your own Facebook App ID
  window.fbAsyncInit = function() {
    FB.init({
      appId      : '...', // FB App ID
      status     : true, // check login status
      cookie     : true, // enable cookies to allow the server to access the session
      xfbml      : true  // parse XFBML
    });

    // Additional initialization code here
  };

  // Load the SDK Asynchronously
  (function(d){
     var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
     js = d.createElement('script'); js.id = id; js.async = true;
     js.src = '//connect.facebook.net/en_US/all.js';
     d.getElementsByTagName('head')[0].appendChild(js);
   }(document));
</script>


<a href="javascript:void(0);" id="login">Create user after logging into Facebook</a>

<div id="fb-root"></div>

<script type="text/javascript">
function login() {
  //Login with Facebook
  FB.login(function(response) {
    if (response.authResponse) {
      var accessToken = response.authResponse.accessToken;

      FB.api('/me', function(response) {
        var user = new StackMob.User({ username: response.email });
        user.createUserWithFacebook(accessToken);
      });

    } else {
      console.log('User cancelled login or did not fully authorize.');
    }
  }, {scope: 'email'});
}

  $('#login').click(function() {
    login();
  });

</script>

</body>
</html>
```


## linkUserWithFacebook

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
linkUserWithFacebook(facebookAccessToken, options)

facebookAccessToken - the user's Facebook Access Token after the user logs into Facebook with the Facebook JS SDK
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

If you already have a StackMob user saved, use this to link that existing user with their Facebook account.  This way they can login with their regular StackMob username/password in the user schema or with their Facebook login.

**Usage:**

```javascript
<html xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
	StackMob.init({
		publicKey:        'your_public_key',
  	apiVersion:        0
  });
/* ]]> */
</script>

</head>
<body>
<script>
  //What follows is standard Facebook JS initialization.  See Facebook for more details.
  //Facebook JS SDK code here.  Use your own Facebook App ID
  window.fbAsyncInit = function() {
    FB.init({
      appId      : '...', // FB App ID
      status     : true, // check login status
      cookie     : true, // enable cookies to allow the server to access the session
      xfbml      : true  // parse XFBML
    });

    // Additional initialization code here
  };

  // Load the SDK Asynchronously
  (function(d){
     var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
     js = d.createElement('script'); js.id = id; js.async = true;
     js.src = '//connect.facebook.net/en_US/all.js';
     d.getElementsByTagName('head')[0].appendChild(js);
   }(document));
</script>


<a href="javascript:void(0);" id="login">Create user after logging into Facebook</a>

<div id="fb-root"></div>

<script type="text/javascript">
function login() {
  //Login with Facebook
  FB.login(function(response) {
    if (response.authResponse) {
      var accessToken = response.authResponse.accessToken;

      FB.api('/me', function(response) {

        var user = new StackMob.User({ username: response.email });
        user.linkUserWithFacebook(accessToken, false);

      });

    } else {
      console.log('User cancelled login or did not fully authorize.');
    }
  }, {scope: 'email'});
}

  $('#login').click(function() {
    login();
  });

</script>

</body>
</html>
```

## unlinkUserFromFacebook

**Introduced:** JS SDK Version 0.9.0

**Description:**

```javascript
unlinkUserFromFacebook( options )

options (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Unlink an existing user with their Facebook account.

**Usage:**

```javascript
    var user = new StackMob.User({ username: 'userName' });
    user.unlinkUserFromFacebook({
      success: function(){
        // Successfully unlinked
      },
      error: function(){
        // Error
      }
    });
```

## loginWithFacebookAutoCreate

**Introduced:** JS SDK Version 0.9.0

**Description:**

```javascript
loginWithFacebookAutoCreate: function(facebookAccessToken, keepLoggedIn, optionsHash) {

facebookAccessToken - the user's Facebook Access Token after the user logs into Facebook with the Facebook JS SDK
keepLoggedIn - This is currently not supported but will be in future versions.  Ignore at this time.
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

This is a convenience method that combines `loginWithFacebook` and `createUserWithFacebook`.  If this user does not already exist, it will be created. Then, the user will be logged in.

**Usage**

Just like above, first get a Facebook Access Token from Facebook, then call `loginWithFacebookAutoCreate`:

```javascript,6
    FB.login(function(response) {
      if (response.authResponse) {

        var accessToken = response.authResponse.accessToken;
        var user = new StackMob.User();
        user.loginWithFacebookAutoCreate(accessToken, false);

      } else {
        console.log('User cancelled login or did not fully authorize.');
      }
    }, {scope: 'email'});
```

## loginWithFacebook

**Introduced:** JS SDK Version 0.9.0

A shorthand method name for `loginWithFacebookToken`.  See `loginWithFacebookToken` below.

## loginWithFacebookToken

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
loginWithFacebookToken: function(facebookAccessToken, keepLoggedIn, optionsHash) {

facebookAccessToken - the user's Facebook Access Token after the user logs into Facebook with the Facebook JS SDK
keepLoggedIn - Tell the sdk whether or not to use refresh tokens and keep the user logged in
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Login a StackMob user with their Facebook account (using their Facebook Access token that the Facebook JS SDK returns upon logging in.).

* This only works with a StackMob user that has already been associated with a Facebook account via `createUserWithFacebook` or `linkUserWithFacebook`

**Usage:**

```javascript
<?DOCTYPE html>
<html xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script type="text/javascript" src="http://static.stackmob.com/js/stackmob-js-0.9.1-bundled-min.js"></script>

<script type="text/javascript">
/* <![CDATA[ */
	StackMob.init({
		publicKey:        'your_public_key',
  	apiVersion:       0
  });
/* ]]> */
</script>

</head>
<body>
<script>
  //What follows is standard Facebook JS initialization.  See Facebook for more details.
  //Facebook JS SDK code here.  Use your own Facebook App ID
  window.fbAsyncInit = function() {
    FB.init({
      appId      : '...', // FB App ID
      status     : true, // check login status
      cookie     : true, // enable cookies to allow the server to access the session
      xfbml      : true  // parse XFBML
    });

    // Additional initialization code here
  };

  // Load the SDK Asynchronously
  (function(d){
     var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
     js = d.createElement('script'); js.id = id; js.async = true;
     js.src = '//connect.facebook.net/en_US/all.js';
     d.getElementsByTagName('head')[0].appendChild(js);
   }(document));
</script>


<a href="javascript:void(0);" id="login">Log into Facebook</a>

<div id="fb-root"></div>

<script type="text/javascript">
function login() {
  //Login with Facebook
  FB.login(function(response) {
    if (response.authResponse) {

      var accessToken = response.authResponse.accessToken;
      var user = new StackMob.User();
      user.loginWithFacebookToken(accessToken, false);

    } else {
      console.log('User cancelled login or did not fully authorize.');
    }
  }, {scope: 'email'});
}

  $('#login').click(function() {
    login();
  });

</script>

</body>
</html>
```

# StackMob.Model.Query

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
select(field)

field - the field you want to return from StackMob
```

Returning a whole object can be costly in terms of size and the time it takes to transfer it.  You can specify which fields to return, like a "SELECT" sql statement.

Selecting fields on expanded objects is also supported.

The primary key is always returned, even if it's not explicitly selected.

**Usage:**

```javascript
  //NOT USING SELECT
  var user = new StackMob.User( { username: 'Chuck Norris' });
  user.query(q);
  
  /* Returns:
 
 {
 	username: 'Chuck Norris',
 	createddate: ...,
 	lastmoddate: ...,
 	age: 50,
 	friends: ['john', 'jane', 'bob']
 }
   
 */
  
  
  //USING SELECT
  var q = new StackMob.Model.Query();
  q.select('username').select('age');
  user.query(q);

  /* Returns:
 {
 	username: 'Chuck Norris',
 	age: 50
 }
 */

  //USING SELECT ON EXPANDED OBJECTS
  var q = new StackMob.Model.Query();
  q.setExpand(1);
  q.select('age').select('friends').select('friends.age');
  user.query(q);
	
  /* Returns:
 {
 	username: 'Chuck Norris', //primary key of an object is always returned, even if not specified by "select"
 	age: 50,
 	friends: [ { age: 30 }, { age: 40 }, { age: 21 } ]
 }
 */
```

# StackMob.Collection

**Introduced:** JS SDK Version 0.1.0

**Description:**

Collections give you the ability to easily manipulate an array (collection) of Model objects.  They're an extension of <a href="http://documentcloud.github.com/backbone/#Collection" target="_blank">Backbone.js Collections</a>, so you can use any method that a <a href="http://documentcloud.github.com/backbone/#Collection" target="_blank">Backbone.js Collection</a> has as well.   There are also some useful <a href="#StackMob.Collection.Query">StackMob queries</a>for Collections.

```javascript
var Books = StackMob.Collection.extend({
  model: Book //refers to "Book" as defined above in "Defining Object/Classes"
});
```

**Usage:**

```javascript
  // Declare this once on the page
  var Book = StackMob.Model.extend({ schemaName: 'book' });
  var Books = StackMob.Collection.extend({ model: Book });


  var books = new Books();
  books.fetch(); //gets all books from StackMob asynchronously
```

## createAll - StackMob.Collection

**Introduced:** JS SDK Version 0.9.0

**Description:**

```javascript
createAll(optionsHash)

optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Batch create all objects in the collection with one API call.

**Usage:**

```javascript
  // Assume we have a book schema with a `title` and `pages` field:
  var Book  = StackMob.Model.extend({ schemaName: 'book' });
  var Books = StackMob.Collection.extend({ model: Book });

  // Prepare our Book objects
  var book1 = new Book({ title: 'Book One',   pages: 800 });
  var book2 = new Book({ title: 'Book Two',   pages: 400 });
  var book3 = new Book({ title: 'Book Three', pages: 460 });
  var book4 = new Book({ title: 'Book Four',  pages: 900 });

  // Put all of the books into a collection
  var booksToCreate = new Books([book1, book2, book3, book4]);

  // Create all of the books in one API call
  booksToCreate.createAll({
    success: function(model) {
      console.log(model);
      /*

        Here, model will contain the ids of succeeded and failed creations:

        model : {
          succeeded: [
            "00e85e16f1d443389c637e6eb82f2a7f",
            "15c54a36bb4f4b21862a9fcd5b52b0b2",
            "896b7b3f9cd844e3bb644d0844e9d104",
            "f9bedd9f288b42118f0ea5f4c55691ac"
          ],
          failed: []
        }
       */
    },
    error: function(model, response) {
      console.log(response);
    }
  });
```


## destroyAll - StackMob.Collection

**Introduced:** JS SDK Version 0.9.0

**Description:**

```javascript
destroyAll(stackmobCollectionQuery, optionsHash)

stackmobCollectionQuery - a StackMob.Collection.Query representing your query
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Batch destroy all objects in the collection with one API call.

**Usage:**

```javascript
  // Assume we have a book schema with a `title` and `pages` field:
  var Book  = StackMob.Model.extend({ schemaName: 'book' });
  var Books = StackMob.Collection.extend({ model: Book });
  var books = new Books();

  var q = new StackMob.Collection.Query();

  // Let's delete all books with more than 450 pages
  q.gt('pages', 450);

  books.destroyAll(q, {
    success: function(model) {
      // model is undefined
    },
    error: function(model, response) {
      console.debug(response);
    }
  });
```

## count - StackMob.Collection

**Introduced:** JS SDK Version 0.2.0

**Description:**

```javascript
count(stackmobModelQuery, optionsHash)

stackmobModelQuery - a StackMob.Model.Query representing your query
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Get the count of the number of results for a given query.


**Usage:**

```javascript
  //Define your Todo class once on the page.
  var Todo = StackMob.Model.extend({
    schemaName: 'blackjack_player'
  });

  //Now, create a collection object and attach it to your previously created model (i.e. Todo)
  var Items = StackMob.Collection.extend({
    model: Todo
  });

  //create an instance of your Item collection
  var itemInstance = new Items();

  //set up the query you want to get the count for
  var q = new StackMob.Collection.Query();
  q.isNotNull('test');

  //call count
  itemInstance.count(q, {
    success: function(count) {
      console.log('count is:' + count );
    }

  });
```

## query - StackMob.Collection

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
query(stackmobCollectionQuery, optionsHash)

stackmobCollectionQuery - a StackMob.Collection.Query representing your query
optionsHash (optional) - a hash containing, but not limited to, optional "success" and "error" functions to execute as callbacks
```

Fetches an array of results from the server depending on the query specified

**Usage:**

```javascript
  var users = new StackMob.Users();

  var q = new StackMob.Collection.Query();
  q.equals('age', 30);
  users.query(q, {
    success: function(collection, response, options) {
      console.debug(collection.toJSON());
      /*
        Outputs:

        [ { username: 'person a', age: 30, ...}, { username: 'person b', age: 30, ...}, { username: 'person c', age: 30, ...}]
      */
    }
  });
```

## Backbone.Collection Methods

StackMob.Collection extends Backbone.Collection and hence inherits all methods from Backbone.Collection.

View <a href="http://documentcloud.github.com/backbone/#Collection" target="_blank">API Docs for Backbone.Collection</a>.


# StackMob.Collection.Query

## and

**Introduced:** JS SDK Version 0.9.0

The <code>and</code> operator is only needed in conjunction with the <code>or</code> operator.  By default, chaining multiple queries will apply an <code>and</code> operator between them.  

See <a href="#a-or">the OR operator for instructions on building more complex queries</a>.

## equals

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
equals(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing
```

Builds a query where the results must conform to a `fieldName` being equal to `value`.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.equals('age', 25);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is 25
```

## lt (less than)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
lt(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing
```

Builds a query where the results must conform to a `fieldName` being less than `value`.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.lt('age', 30);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is less than 30
```

## lte (less than equal to)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
lte(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing to
```

Builds a query where the results must conform to a `fieldName` being less than or equal to `value`.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.lte('age', 30);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is less than or equal to 30
```

## gt (greater than)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
gt(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing
```

Builds a query where the results must conform to a `fieldName` being greater than `value`.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.gt('age', 30);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is greater than 30
```

## gte (greater than equal to)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
gte(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing
```

Builds a query where the results must conform to a `fieldName` being greater than or equal to `value`.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.gte('age', 30);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is greater than or equal to 30
```

## isNull

**Introduced:** JS SDK Version 0.2.1

**Description:**

```javascript
isNull(fieldName)

fieldName - The field name you are comparing
```

Builds a query where the results must conform to a `fieldName` being null.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.isNull('age');

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is set to null
```

## isNotNull

**Introduced:** JS SDK Version 0.2.0

**Description:**

```javascript
isNotNull(fieldName)

fieldName - The field name you are comparing
```

Builds a query where the results must conform to a `fieldName` not being null.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.isNotNull('age');

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is not null
```

## mustBeOneOf

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
mustBeOneOf(fieldName, value)

fieldName - The field name you are comparing.  Can be also be a fieldName representing an array.
value - The value or array of values you want to match on
```

Builds a query where the results' field must match at least one of the value(s) specified in `value`.  If the field is an array, the result will be returned if one item in the array matches `value`.


**Usage:**

```javascript
  //"mustBeOneOf" for a single numeric value
  var q = new StackMob.Collection.Query();
  q.mustBeOneOf('age', 100);

  var centennials = new StackMob.Users();
  centennials.query(q); //fetch all users whose age is 100



  //"mustBeOneOf" for multiples numeric values
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.mustBeOneOf('age', [25,30,40]);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is 25, 30, or 40



  //"mustBeOneOf" for multiples String values
  var cerealQuery = new StackMob.Collection.Query();
  cerealQuery.mustBeOneOf('favoritecereals', ['fruit loops', 'frosted flakes']);

  var cerealLovingUsers = new StackMob.Users();
  cerealLovingUsers.query(cerealQuery); //fetch all users who's favoritecereals include "fruit loops" and/or "frosted flakes"
```

## mustNotBeOneOf

**Introduced:** JS SDK Version 0.9.3

**Description:**

```javascript
mustNotBeOneOf(fieldName, value)

fieldName - The field name you are comparing.  Can be also be a fieldName representing an array.
value - The value or array of values you want to match on.
```

Builds a query where the results' field must not match any of the value(s) specified in `value`.  If the field is an array, the result will be returned if none of the items in the array matches `value`.


**Usage:**

```javascript
  //"mustNotBeOneOf" for a single numeric value
  var q = new StackMob.Collection.Query();
  q.mustNotBeOneOf('age', 100);

  var centennials = new StackMob.Users();
  centennials.query(q); //fetch all users whose age is 100



  //"mustNotBeOneOf" for multiples numeric values
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.mustNotBeOneOf('age', [25,30,40]);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is 25, 30, or 40



  //"mustNotBeOneOf" for multiples String values
  var cerealQuery = new StackMob.Collection.Query();
  cerealQuery.mustNotBeOneOf('favoritecereals', ['fruit loops', 'frosted flakes']);

  var cerealLovingUsers = new StackMob.Users();
  cerealLovingUsers.query(cerealQuery); //fetch all users who's favoritecereals include "fruit loops" and/or "frosted flakes"
```

## notEquals

**Introduced:** JS SDK Version 0.2.0

**Description:**

```javascript
notEquals(fieldName, value)

fieldName - The field name you are comparing
value - The value you are comparing
```

Builds a query where the results must conform to a `fieldName` not being equal to value.


**Usage:**

```javascript
  var stackmobQuery = new StackMob.Collection.Query();
  stackmobQuery.notEquals('age', 30);

  var users = new StackMob.Users();
  users.query(stackmobQuery); //fetch all users whose age is not equal to 30
```

## or

**Introduced:** JS SDK Version 0.9.0

Combine a Query with an OR operator between it and the current Query object.

**Description:**

```javascript
    or( query )

    query - a StackMob.Collection.Query objec to OR the current query with
```

**Usage:**

Take a query like this for example:

(age > 25 && ((location == NYC && name != john) || (location == SF && name != Mary) || location == LA))

To build this query in JavaScript, it would look like this

```javascript
    var isAged  = new StackMob.Collection.Query().equals("age", "25");
    var isNYC   = new StackMob.Collection.Query().equals("location", "NYC");
    var notJohn = new StackMob.Collection.Query().notEquals("name", "john");
    var notMary = new StackMob.Collection.Query().equals("location", "SF").notEquals("name", "mary");
    var isLA    = new StackMob.Collection.Query().equals("location", "LA");
    isAged.and( notJohn.or(notMary).or(isLA) );
```

For more detailed information on OR Query support, read the <a href="https://developer.stackmob.com/sdks/rest/api#a-or_and_and_queries">REST API documentation here</a>.

## orderAsc

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
orderAsc(fieldName)

fieldName - The field name you want to sort by in ascending order
```

Builds a query where the results must be sorted by the fieldName value in ascending order


**Usage:**

```javascript
  //sort by "age" in ascending order
  var q = new StackMob.Collection.Query();
  q.orderAsc('age');


  var users = new StackMob.Users();
  users.query(q); //fetch all users and order them by age in ascending order



  //sort by "age" and then "username" in ascending order
  var q = new StackMob.Collection.Query();
  q.orderAsc('age').orderAsc('username);

  var users = new StackMob.Users();
  users.query(q); //fetch all users and order them by age in ascending order.  If it's a tie, sort by their username in alphabetical order from 'a' to 'z'.
```

## orderDesc

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
orderDesc(fieldName)

fieldName - The field name you want to sort by in descending order
```

Builds a query where the results must be sorted by the fieldName value in descending order


**Usage:**

```javascript
  //sort by "age" in descending order
  var q = new StackMob.Collection.Query();
  q.orderDesc('age');

  var users = new StackMob.Users();
  users.query(q); //fetch all users and order them by age in descending order



  //sort by "age" and then "username" in descending order
  var q = new StackMob.Collection.Query();
  q.orderDesc('age').orderDesc('username);

  var users = new StackMob.Users();
  users.query(q); //fetch all users and order them by age in descending order.  If it's a tie, sort by their username in alphabetical order from 'z' to 'a'.
```

## setRange

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
setRange(start, end)

start - the starting number of the result set portion you want returned
end - the ending number of the result set portion you want returned
```

For pagination, you may want a certain range of results.  Set that range here with the start and end.  For example, showing 10 results per page, page 2 would have a start and end of 10 and 19 respectively.


**Usage:**

```javascript
  var q = new StackMob.Collection.Query();
  q.lt('age', 50).setRange(0,14).orderAsc('username');

  var users = new StackMob.Users();
  users.query(q); //fetch the first 15 users less than age 50, sorted by "username" ascending from 'a' to 'z'
```

## setExpand

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
setExpand(depth)

depth - how "deeply" you want to recursively expand related objects
```

If your object has a <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationships</a>, the <a href="https://www.stackmob.com/devcenter/docs/Schema-Relationships" target="_blank">Relationships</a> is represented by a value or array of primary keys.  You can expand the results so that the full object or array of full objects is returned instead of the primary key.

Similarly, your related objects may have relationships themselves.  To expand those, set expand depth to level "2".  The maximum expansion depth is "3".


**Usage:**

```javascript
  var q = new StackMob.Collection.Query();
  q.setExpand(1);

  var users = new StackMob.Users();
  users.query(q); //fetch users and expand their relationships to return full objects instead of their primary key
  /*
    Unexpanded:

    [
      {
        username: 'Bodie',
        age: 1,
        friends: ['Kodi', 'Herc'],
        ...
      },
      {
        username: 'Nola',
        age: 2,
        friends: ['Kodi'],
        ...
      }
    ]


    Expanded Depth 1:

    [
      {
        username: 'Bodie',
        age: 1,
        friends: [
          {
            username: 'Kodi',
            age: 2,
            friends: [ 'Bodie', 'Nola' ],
            ...
          },
          {
            username: 'Herc',
            age: 100,
            friends: [ 'Archer', 'Henry' ],
            ...
          }
        ],
        ...
      },
      {
        username: 'Nola',
        age: 35,
        friends: [{
            username: 'Kodi',
            age: 2,
            friends: [ 'Bodie', 'Nola' ],
            ...
          }],
        ...
      }
    ]
   */
```

## select

**Introduced:** JS SDK Version 0.1.1

**Description:**

```javascript
select(field)

field - the field you want to return from StackMob
```

Returning a whole object can be costly in terms of size and the time it takes to transfer it.  You can specify which fields to return, like a "SELECT" sql statement.

Selecting fields on expanded objects is also supported.

**Usage:**

```javascript
	//NOT USING SELECT
  var users = new StackMob.Users();
  users.fetch();
  
  /* Returns:
   
  	[
   		{
   			username: 'Chuck Norris',
   			createddate: ...,
   			lastmoddate: ...,
   			age: 50,
   			friends: ['john', 'jane', 'bob']
   		},
   		{
   			username: 'Marty McFly',
   			createddate: ...,
   			lastmoddate: ...,
   			age: 25
   			friends: ['john', 'jane', 'bob']
   		}
  	] 
   
   */
  
  

  //USING SELECT
  var q = new StackMob.Collection.Query();
  q.select('username');
  users.query(q);

	/* Returns:
  	[
   		{
   			username: 'Chuck Norris'
   		},
   		{
   			username: 'Marty McFly'
   		}
  	]	 
	 */

	//USING SELECT ON EXPANDED OBJECTS
	var q = new StackMob.Collection.Query();
	q.setExpand(1);
	q.select('username').select('friends.age');
	users.query(q);
	
	/* Returns:
  	[
   		{
   			username: 'Chuck Norris',
   			friends: [ { age: 30 }, { age: 40 }, { age: 21 } ]
   		},
   		{
   			username: 'Marty McFly',
   			friends: [ { age: 30 }, { age: 40 }, { age: 21 } ]
   		}
  	]	 
	 */
```


# StackMob.GeoPoint

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
var geopoint = new StackMob.GeoPoint(lat, lon);

lat: the latitude coordinates. Range: -90,90 (both inclusive)
lon: the longitude coordinates. Range: -180 (inclusive), 180 (exclusive)
```

A StackMob.GeoPoint is a simple wrapper around `lat` and `lon` coordinates.  It's used in geolocation queries.  (See below).

**Usage:**

Saving a `StackMob.GeoPoint`.  This example assumes you have a `geopoint` field type of name `location` in an object called `Place`:

```javascript
<script type="text/javascript">
//Example object of name "Place". You can choose any name you'd like.
var Place = new StackMob.Model.extend({ schemaName: 'place' }); 

var latlon = new StackMob.GeoPoint(37.772161,-122.406443); //lat, lon
var stackmob_office = new Place({ location: latlon.toJSON() }); //notice "toJSON()"
stackmob_office.create();

//Or you can save a geopoint with JSON, though StackMob.GeoPoint is recommended.
var same_stackmob_office = new Place({ location: { lat: 37.772161, lon: -122.406443 }});
same_stackmob_office.create();
</script type>
```

## toJSON

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
toJSON()

Returns a JSON representation of the lat long coordinates:

{
  lat: 37.772161,
  lon: -122.406443
}
```

Converts a StackMob.GeoPoint to JSON.  This is useful when creating object instances.

**Usage:**

```javascript
<script type="text/javascript">
//Example object "Place". You can choose any name you'd like
var Place = new StackMob.Model.extend({ schemaName: 'place' }); 

var latlon = new StackMob.GeoPoint(37.772161,-122.406443); //lat, lon
var stackmob_office = new Place({ location: latlon.toJSON() }); //notice "toJSON()"
stackmob_office.create();
</script type>
```

# StackMob.Collection.Query with StackMob.GeoPoint


See more below on using `StackMob.Collection.Query` with `StackMob.GeoPoint`.


## mustBeNear (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
mustBeNear(field, smGeoPoint, distanceRadians)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceRadians - distance from geopoint in radians
```

Search for objects near the `smGeoPoint`, with distance specified in radians


**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.mustBeNear('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## mustBeNearMi (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
mustBeNearMi(field, smGeoPoint, distanceMiles)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceMiles - distance from geopoint in miles
```

Search for objects near the `smGeoPoint`, with distance specified in miles


**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.mustBeNearMi('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## mustBeNearKm (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
mustBeNearKm(field, smGeoPoint, distanceKilometers)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceKilometers - distance from geopoint in kilometers
```

Search for objects near the `smGeoPoint`, with distance specified in miles


**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.mustBeNearKm('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## isWithin (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
isWithin(field, smGeoPoint, distanceRadians)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceRadians - distance from geopoint in radians
```

Search for objects within the `smGeoPoint`, with distance specified in radians

**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.isWithin('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## isWithinMi (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
isWithinMi(field, smGeoPoint, distanceMiles)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceMiles - distance from geopoint in miles
```

Search for objects within the `smGeoPoint`, with distance specified in miles

**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.isWithinMi('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## isWithinKm (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
isWithinKm(field, smGeoPoint, distanceKilometers)

field - the field representing the geolocation you're searching on
smGeoPoint - the geopoint from which you're measuring the distance
distanceKilometers - distance from geopoint in kilometers
```

Search for objects within the `smGeoPoint`, with distance specified in kilometers

**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.isWithinKm('location', new StackMob.GeoPoint(37.77493,-122.419416), 1);

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```

## isWithinBox (geo)

**Introduced:** JS SDK Version 0.1.0

**Description:**

```javascript
isWithinBox(field, bottomLeftGeoPoint, topRightGeoPoint)

field - the field representing the geolocation you're searching on
bottomLeftGeoPoint - the geopoint representing the bottom left of the box
topRightGeoPoint - the geopoint representing the top right of the box
```

Search for objects within a box specified by the bottom left and top right coordinates of

**Usage:**

```javascript
  var Attraction = StackMob.Model.extend({ schemaName: 'attraction' });
  var Attractions = StackMob.Collection.extend({ model: Attraction });

  var q = new StackMob.Collection.Query();
  q.isWithinBox('location', new StackMob.GeoPoint(37.77493,-122.419416), new StackMob.GeoPoint(37.783367,-122.408579));

  var attractions = new Attractions();
  attractions.query(q, {
    success: function(collection) {
    	console.debug(collection.toJSON());
    }
  });
```
