<h3>Objective</h3>
Create a GeoPoint object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-GeoPoint-Field-to-Schemas" target="_blank">You have a schema that has a GeoPoint field</a>

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.geopoint" target="_blank">StackMob.GeoPoint</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-tojson" target="_blank">toJSON</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.collection.query_with_stackmob.geopoint" target="_blank">GeoPoint Query</a>

<h2>Create a GeoPoint Object</h2>

This tutorial assumes you have a schema called `place` and a GeoPoint field called `location` in that schema

```js
// If your schema name is different than `place`, then you need to replace `place` with your schema
var Place = StackMob.Model.extend({ schemaName: 'place' });
var Places = StackMob.Collection.extend({ model: Place });

// GeoPoint(lat, lon) has some constraints:
//  lat: -90,90 (both inclusive)
//  lon: -180 (inclusive), 180 (exclusive)
var latlon = new StackMob.GeoPoint(37.772161, -122.406443);

var q = new StackMob.Collection.Query();
q.mustBeNear('location', latlon, 1);

var placesContainer = new Places();
placesContainer.query(q, {
  success: function(model) {
    console.debug(model);
  },
  error: function(model, response) {
    console.debug(response);
  }
});
```

There are other GeoPoint queries that you can use and they are documented <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-stackmob.collection.query_with_stackmob.geopoint" target="_blank">here</a>