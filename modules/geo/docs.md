StackMob Geolocation
=====================================

StackMob's Geolocation module integrates with your StackMob datastore to give you the ability to save and query geolocation data.  It introduces a new `geopoint` field type to your Schema field options.

You are currently limited to one geopoint field per schema.  (Of course, you aren't limited by the number of schemas you have in your app, however.)

Let's add a geopoint to your schema and then use it from the SDKs.

## Setup

Go to <a href="https://dashboard.stackmob.com/schemas/">Manage Schemas</a> and click `Edit` link on one of the schemas that you would like to add a GeoLocation field.

Click Add Field button and in this example, we will name our GeoPoint field as `location`.

<img src="//s3.amazonaws.com/static.stackmob.com/images/tutorial/fill_add_field.png"></img>

Click Add Field, then Save Schema, and you're set!

## Saving and Querying Geolocation data

<a href="https://developer.stackmob.com/sdks">Check your SDK's</a> Developer Guide for more info on saving and querying.



<p class="alert">
	You can only have one geopoint field per schema.
</p>
