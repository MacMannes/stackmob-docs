<h3>Objective</h3>
Call a custom code method from the JS SDK

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>
* <a href="https://developer.stackmob.com/tutorials/customcode/Build-and-Upload-Custom-Code-Example" target="_blank">Build and Upload Custom Code Example</a>  

For a more in-depth look into Custom Code, you may want to check out a detailed introduction here:
* <a href="http://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK" target="_blank">Getting Started: Custom Code SDK</a>

<h2>Related API Documentation</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-customcode" target="_blank">StackMob.customcode</a>

<h1>Let's get started!</h1>

<h2>Build and Upload your Custom Code</h2> 
You'll need to build and upload a JAR based on the StackMob Custom Code examples.

We've created a separate tutorial called <a href="https://developer.stackmob.com/tutorials/customcode/Build-and-Upload-Custom-Code-Example">Build and Upload Custom Code Example</a>. If you haven't done this step before, complete that tutorial before continuing.

<h2>Call a method</h2>

Now that you have the example custom code uploaded to StackMob and running, let's call it from Javascript.  Assuming you have
 <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">configured the JS SDK on your page already</a>, you can call 
 custom code methods like this:

```javascript

  StackMob.customcode('hello_world', // custom code method
    {}, // method parameters
    'GET', // HTTP Verb - omit this parameter for 'GET' by default
    {
      success: function(jsonResult) {
        //jsonResult is the JSON object: { "msg": "Hello, world!" }
        alert( JSON.stringify(jsonResult) );
      },

      error: function(failure) {
        // ...
      }
    }
  );
```
Then save this file as customcode.html

<h2>Test it</h2>
Run the `stackmobserver.py` from the same directory as `index.html` so that your web server is started.  Visit <a href="http://localhost:4567/customcode.html" target="_blank">http://localhost:4567/customcode.html</a>, which will call your custom code that's hosted on StackMob.

You should get a popup with the text returned from custom code.

Congratulations of finishing this tutorial!
