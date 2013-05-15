# JavaScript: Parse and StackMob

<a href="https://www.stackmob.com/parse/">&larr; Back to Migrate from Parse</a>

# Introduction

Coming from Parse to StackMob via JavaScript is incredibly easy in that both JavaScript SDKs are based off of <a href="http://backbonejs.org" target="_blank">Backbonejs.org</a>.  You can browse <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" target="_blank">StackMob's JS API Docs</a> and <a href="http://developer.stackmob.com/tutorials/js" target="_blank">StackMob JS Tutorials</a> for more detailed information, but for now, let's get down into a quick look as to how easily you can move to StackMob.

# Declaring custom classes

At Parse, you declare a custom class with:

```js
var GameScore = Parse.Object.extend("GameScore");
```

With StackMob, it's:

```js
var GameScore = StackMob.Model.extend({ schemaName: "GameScore" });
```

# Creating an object

With Parse, you create an object via:

```js
var gameScore = new GameScore();
 
gameScore.set("score", 1337);
gameScore.set("cheatMode", false);
 
gameScore.save(null, {
  success: function(gameScore) {},
  error: function(gameScore, error) {}
});
```

And with StackMob, it's practically the same.

```js
var gameScore = new GameScore();

gameScore.set("score", 1337);
gameScore.set("cheatMode", false);

gameScore.create({
	success: function(model, result, options) {},
	error: function(model, error, options) {}
});
```

# Querying for objects

To get an object with Parse:

```js
var GameScore = Parse.Object.extend("GameScore");
var query = new Parse.Query(GameScore);
query.get("xWMyZ4YEGZ", {
  success: function(gameScore) {},
  error: function(object, error) {}
});
```

With StackMob:

```js
var GameScore = StackMob.Model.extend({ schemaName: "GameScore" });
var gamescore = new GameScore({ gamescore_id: "xWMyZ4YEGZ"});
gamescore.fetch({
	success: function() {},
	error: function() {}
});
```

To get multiple objects with Parse:

```js
var GameScore = Parse.Object.extend("GameScore");
var query = new Parse.Query(GameScore);
query.equalTo("playerName", "erick");
query.find({
  success: function(results) {
    alert("Successfully retrieved " + results.length + " scores.");
  },
  error: function(error) {
    alert("Error: " + error.code + " " + error.message);
  }
});
```

With StackMob:

```js
var GameScore = StackMob.Model.extend({ schemaName: "GameScore"});
var GameScores = StackMob.Collection.extend({ model: GameScore });

var scores = new GameScores();

var q = new StackMob.Collection.Query();
q.equals('playerName', 'erick');

scores.query(q, {
	success: function(collection) {},
	error: function(collection, result, options) {}
});
```


# Users

To create a new user on Parse:

```js
var user = new Parse.User();
user.set("username", "erick");
user.set("password", "stackmob");
user.set("email", "email@example.com");
user.set("phone", "415-392-0202");
 
user.signUp(null, {
  success: function(user) {},
  error: function(user, error) {}
});
```

And with StackMob, it's similar again:

```js
var user = new StackMob.User({ username: 'erick', password: 'stackmob' });

//you can also use set
user.set('email', 'email@example.com');
user.set('phone', '415-392-0202');

user.create({
	success: function(model, result, options) {},
	error: function(model, error, options) {}
});
```

## Facebook Users

Let's login a user with Facebook on Parse:

```js
//First do some things with the Facebook JS SDK

//Then..

Parse.FacebookUtils.logIn(null, {
  success: function(user) {
    if (!user.existed()) {
      alert("User signed up and logged in through Facebook!");
    } else {
      alert("User logged in through Facebook!");
    }
  },
  error: function(user, error) {
    alert("User cancelled the Facebook login or did not fully authorize.");
  }
});
```

and on StackMob:

```js
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


# GeoPoints

Declaring geopoints in Parse is:

```js
var point = new Parse.GeoPoint({latitude: 40.0, longitude: -30.0});
```

And easy with StackMob:

```js
var latlon = new StackMob.GeoPoint(37.772161,-122.406443); //lat, lon

//and then saving it is
var stackmob_office = new Place({ location: latlon.toJSON() }); //notice "toJSON()"
stackmob_office.create();
```

# Custom Code

Parse has Cloud Code, which runs JavaScript serverside.  You can write server-side StackMob's Custom Code in Java and Scala and call it as well.

With Parse, call Cloud Code with:

```js
Parse.Cloud.run('hello', {}, {
  success: function(result) {},
  error: function(error) {}
});
```

Call StackMob's Custom Code with:

```js
StackMob.customcode('hello', {}, {
  success: function(result) {},
  error: function(result) {}
});
```

# Easy!

As you can see, you'll be able to pick up running with StackMob!  In fact, not only are the SDKs similar, but StackMob's relationship support and Access Control Levels are even better.  Learn more by looking through <a href="http://developer.stackmob.com/tutorials/js" target="_blank">StackMob's JavaScript Tutorials</a>!
