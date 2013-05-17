<h3>Objective</h3>
Fetching parameters JSON and non-JSON in custom code

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~15 minutes

<h3>Prerequisites</h3>

* An application in StackMob. If you don't have any, you need to <a href="https://developer.stackmob.com/start" target="_blank">create an application</a>.

* Custom code with 2 parameters uploaded into your app. If you don't know how, follow these <a href="https://developer.stackmob.com/tutorials/customcode/Getting-Started:-Custom-Code-SDK#a-quickstart_-_fork_example_custom_code_on_github" target="_blank">instructions to quickly get your custom code up and running</a>.

<h1>Let's get started!</h1>

<h2>Related API</h2>

* None in particular, but see our <a href="http://stackmob.github.com/stackmob-customcode-sdk/0.5.0/apidocs/" target="_blank">Custom Code API docs</a> for more details.

<h2>Parse Parameters</h2>

There are two different ways to parse the parameters that get passed in to your custom code methods. One is for `GET` and `DELETE` requests (non-JSON param), and the other one is for `POST` and `PUT` requests (JSON param).

In this tutorial, you will see how to handle these two cases.

If you use `stackmob-custom-code-example` project, all you need to do is add a class for your custom code.

Say you want to have a custom code method called `set_high_score` that accepts two parameters, i.e. `username` and `score` and returns the verb used and the parameters passed into it.

<span class="tab" title="Java" />

Your `java` class should look similar to the code below.

```java,53,67
package com.stackmob.example;

import com.stackmob.core.customcode.CustomCodeMethod;
import com.stackmob.core.rest.ProcessedAPIRequest;
import com.stackmob.core.rest.ResponseToProcess;
import com.stackmob.sdkapi.SDKServiceProvider;

import java.lang.Exception;
import java.lang.String;
import java.lang.StringBuilder;
import java.net.HttpURLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class SetHighScore implements CustomCodeMethod {

  /**
   * Name our custom code method
   */
  @Override
  public String getMethodName() {
    return "set_high_score";
  }

  /**
   * Specify parameters for our custom code method
   */
  @Override
  public List<String> getParams() {
    return new ArrayList<String>() {{
      add("username");
      add("score");
    }};
  }

  @Override
  public ResponseToProcess execute(ProcessedAPIRequest request, SDKServiceProvider serviceProvider) {
    // this logger can be used to log any activity and you can view the log at https://dashboard.stackmob.com/data/logs
    // LoggerService logger = serviceProvider.getLoggerService(SetHighScore.class);

    Map<String, Object> map = new HashMap<String, Object>();
    String verb = request.getVerb().toString();

    StringBuilder sb = new StringBuilder(verb + " =>");

    // this is where we handle the special case for `POST` and `PUT` requests
    if (verb.equalsIgnoreCase("post") || verb.equalsIgnoreCase("put")) {

      if (!request.getBody().isEmpty()) {
        try {
          JSONObject jsonObj = new JSONObject(request.getBody());
          if (!jsonObj.isNull("username")) sb.append(" --username: " + jsonObj.getString("username"));
          if (!jsonObj.isNull("score")) sb.append(" --score: " + jsonObj.getString("score"));
        } catch (JSONException e) {
          sb.append("Caught JSON Exception");
          e.printStackTrace();
        }
      } else sb.append("Request body is empty");
    
    // this is where we handle the case for `GET` and `DELETE` requests
    } else sb.append( String.format("username: %s | score: %s", request.getParams().get("username"), request.getParams().get("score")) );

    map.put("message", sb.toString());
    map.put("verb", verb);
    return new ResponseToProcess(HttpURLConnection.HTTP_OK, map);
  }

}
```

When you do `mvn package`, you might get an error about `org.json`. To solve this, you just need to add a dependency for that in `pom.xml` like this:

```xml
<dependency>
  <groupId>org.json</groupId>
  <artifactId>json</artifactId>
  <version>20090211</version>
</dependency>
```

<span class="tab" />

<span class="tab" title="Scala" />

Your `scala` class should look similar to the code below.

```scala,32,49
package com.stackmob.example

import com.stackmob.core.customcode.CustomCodeMethod
import com.stackmob.sdkapi.SDKServiceProvider
import com.stackmob.core.rest.{ResponseToProcess, ProcessedAPIRequest}
import java.util.{List => JList}
import java.net.HttpURLConnection
import scala.collection.JavaConverters._
import util.parsing.json.JSON

class SetHighScore extends CustomCodeMethod {

  /**
   * Name our custom code method
   */
  override def getMethodName: String = "set_high_score"

  /**
   * Specify parameters for our custom code method
   */
  override def getParams: JList[String] = List[String]("username", "score").asJava

  override def execute(request: ProcessedAPIRequest, serviceProvider: SDKServiceProvider): ResponseToProcess = {
    // this logger can be used to log any activity and you can view the log at https://dashboard.stackmob.com/data/logs
    // val logger = serviceProvider.getLoggerService(classOf[SetHighScore])

    val verb = request.getVerb.toString

    val sb = new StringBuilder(verb + " =>")

    // this is where we handle the special case for `POST` and `PUT` requests
    if (verb.equalsIgnoreCase("post") || verb.equalsIgnoreCase("put")) {

      if (!request.getBody.isEmpty) {
        JSON.parseFull(request.getBody).map(json => {
          json match {
            case x: Map[String,Any] =>
              x.get("username").map(obj => sb.append(" --username: ").append(obj.toString))
              x.get("score").map(obj => sb.append(" --score: ").append(obj.toString))
            case y: List[Any] =>
              sb.append(y.toString)
            case _ =>
              sb.append("unknown data type received")
          }
        })
      }

    // this is where we handle the case for `GET` and `DELETE` requests
    } else sb.append( "username: %s | score: %s".format(request.getParams.get("username"), request.getParams.get("score")) )

    new ResponseToProcess(HttpURLConnection.HTTP_OK, Map("verb" -> verb, "message" -> sb.toString).asJava)
  }
}
```

<span class="tab" />