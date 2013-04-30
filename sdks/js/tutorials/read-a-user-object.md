<h3>Objectives</h3>
Read a `user` object

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://dashboard.stackmob.com/sdks/js/config" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object which has `Bill Watterson` as its `username` (create using <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Object Browser</a> or do <a href="https://developer.stackmob.com/tutorials/js/Create-a-User-Object" target="_blank">create tutorial</a>)

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/sdks/js/api#a-fetch" target="_blank">fetch</a>

<h2>Read a User Object</h2>

Let's try to read `User` data for `Bill Watterson`.

```js
var user = new StackMob.User({ username: 'Bill Watterson' });
user.fetch({
	success: function(model) {
	console.debug(model.get('username') + ': ' + model.get('profession'));
	}
});
```