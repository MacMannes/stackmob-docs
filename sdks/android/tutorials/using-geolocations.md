<h3>Objective</h3>

Save and query against geolocation data.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://dashboard.stackmob.com/sdks/android/config">set up for StackMob</a>

<h1>Let's get started!</h1>

To use geopoints, you first need to create a geopoint field in your schema. Go to [Manage Schemas](), edit your schema, and create a field of type Geopoint.

![Add a Geopoint field](http://static.stackmob.com/images/android/geopoint.png)

For this example we've got a Geopoint field named location on the schema `task` and a corresponding field on our `Task` object of type `StackMobGeoPoint`.

```java
package com.example.yourapp;
import java.util.Date;
import com.stackmob.sdk.model.StackMobModel;

public class Task extends StackMobModel {

	private String name;
	private Date dueDate;
	private int priority;
	private boolean done;
	private StackMobGeoPoint location;

	public Task(String name, Date dueDate) {
		super(Task.class);
		this.name = name;
		this.dueDate = dueDate;
		this.priority = 0;
		this.done = false;
	}


	public void setLocation(StackMobGeoPoint location) {
		this.location = location;
	}

	public StackMobGeoPoint getLocation() {
		return location;
	}
}
```

[StackMobGeoPoint](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobGeoPoint.html) is a longitude/latitude pair representing a point. It uses radians, and you can convert distances to radians via the static methods on `StackMobGeoPoint`. You can now save and query for your `Task` object, and the location field will be treated like any other data. Once you have some objects with geopoints stored you use them for spacially
aware queries. 

<h3>Near Queries</h3>

Near queries allow you to get a list of objects sorted by distance from a given `GeoPoint`. You can optionally specify a maximum distance to search. Any ordering you specify will be ignored. Here's a query to find all `Task` objects
within 10 miles of San Francisco.

```java
StackMobGeoPoint sf = new StackMobGeoPoint(122.68, 37.73);
StackMobQuery q = new StackMobQuery().fieldIsNearWithinMi("location", sf, 10); //(lon, lat), distance (miles)
Task.query(Task.class, q, new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
		for(Task task : result) {
			System.out.println(task.getLocation().getQueryDistanceRadians());
		}
    }

    @Override
    public void failure(StackMobException e) {
    }
});

```
Note how the `StackMobGeoPoint` objects you get back from a near query have a property `getQueryDistanceRadians` available. This is the distance between that point and the reference (in this case San Francisco), which can be handy.


<h3>Within Queries</h3>
Within queries search for results within a specified radius of a given point or within a 2-dimensional box. These queries do not have a natural ordering. Unlike near queries, if you specify an ordering for the result set it will be honored. 

```java
StackMobGeoPoint sf = new StackMobGeoPoint(122.68, 37.73);
StackMobQuery q = new StackMobQuery().fieldIsWithinRadiusInMi("location", sf, 10); //(lon, lat), distance (miles)
Task.query(Task.class, q, new StackMobQueryCallback<Task>() {
    @Override
    public void success(List<Task> result) {
    }

    @Override
    public void failure(StackMobException e) {
    }
});

```

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobModel](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobModel.html), [StackMobQuery](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobQuery.html), and [StackMobGeoPoint](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/api/StackMobGeoPoint.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)
