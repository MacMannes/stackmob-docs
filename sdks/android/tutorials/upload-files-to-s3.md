<h3>Objective</h3>

Upload data to S3 via saving StackMob objects

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://dashboard.stackmob.com/sdks/android/config">set up for StackMob</a>

* <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-Binary-Field-to-Schemas">Add a Binary Field to your schema and add your S3 credentials to the StackMob Dashboard</a>

<h1>Let's get started!</h1>

StackMob lets you save large files to S3 as easily as saving any other data to StackMob's cloud. The files are then accessible via an S3 url. To start, make sure you've [configured your app for S3](https://developer.stackmob.com/tutorials/android/Upload-to-S3). For this example we've got a binary field named photo on the schema `task` and a corresponding field on our `Task` object of type `StackMobFile`.

```java
package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private StackMobFile photo;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}


	public void setPhoto(StackMobFile photo) {
		this.photo = photo;
	}

	public StackMobFile getPhoto() {
		return photo;
	}
}
```

If you set a `StackMobFile` to `photo` and then save your object, 

```java
byte[] image = // ... get the bytes of whatever you're uploading
Task myTask = new Task("Upload some files", new Date());
myTask.setPhoto(new StackMobFile("image/jpeg", "mypicture.jpg", image);
test.save(new StackMobModelCallback() {
    @Override
    public void success() {
        System.out.println(test.getPhoto().getS3Url());
    }

    @Override
    public void failure(StackMobException e) {
    }
});
```
Once the success callback is called, you can check `getS3Url()` to get the url the photo has been uploaded to. Once you've successfully uploaded your file the local data will be cleared to save memory. To delete the object from StackMob and the file from S3, simply call

```java
myTask.destroy()
```

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html) and [StackMobFile](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobFile.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)
