<h3>Objective</h3>
Building, uploading and testing the StackMob custom code examples.

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* You have a StackMob account. If you don't, <a href="https://stackmob.com/signup" target="_blank">sign up</a> now!

* You have a StackMob application


<h1>Let's get started!</h1>

<h2>Downloading the Custom code examples from Github</h2>

Go to the StackMob <a href="https://github.com/stackmob/stackmob-customcode-example" target="_blank">Custom code example Github repository</a> and click the ZIP button.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/customcode/upload-customcode-01.png"></img>
<br>

<h2>Building a JAR file</h2>

You have 3 languages to choose from Java, Scala and Clojure.  Using a terminal application, follow the steps below to build a JAR file.

<h3>Java (Maven)</h3>

	cd java
	mvn clean package`

JAR is located in **java/target**

<h3>Scala (Maven)</h3>

    cd scala-maven
    mvn clean package

JAR is located in **scala/target**


<h3>Scala (sbt)</h3>

    cd scala-sbt
    sbt clean package

JAR is located in **scala/target/scala-2.9.1**


<h3>Clojure (Maven)</h3>

    cd clojure
    mvn clean package

JAR is located in **clojure/target**


<h2>Testing your code</h2>

Once your JAR file is built, <a href="https://dashboard.stackmob.com/module/customcode/view">Upload the JAR</a> through the StackMob dashboard.

Let's test the hello world example using the <a href="https://dashboard.stackmob.com/module/customcode/console">StackMob Test Console</a>.

Click on **hello_world** under the Custom Methods heading.  Click the submit button.  Scroll down the page to see the Response Body.  The response should be:

```json
{
  "error": "Your custom code instance has been scheduled for start, and should be ready shortly" 
}
```

Our SDKs, will automatically retry when encountering this "startup" message.


Click the submit button a second time and you'll see the following.

```json
{
  "msg": "Hello, world!" 
}
```

Congratulations on completing this tutorial!

