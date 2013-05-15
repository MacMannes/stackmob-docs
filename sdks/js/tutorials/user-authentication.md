<h3>Objective</h3>
Login and logout user

<h3>Experience Level</h3>
Beginner

<h3>Estimated Time to Complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/configure" target="_blank">Running the StackMob Python Web Server with your initialized JS SDK</a>

* There is a `User` object with `username: 'test'` and `password: 'test'`

<h1>Let's get started!</h1>

<h2>Related API</h2>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-login" target="_blank">login</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-isloggedin_-_stackmob" target="_blank">isLoggedIn</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-isuserloggedin" target="_blank">isUserLoggedIn</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-getloggedinuser" target="_blank">getLoggedInUser</a>

* <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs#a-logout" target="_blank">logout</a>

<h2>Login With an Existing User</h2>

Let's log in user `test` to StackMob.

```js
var user = new StackMob.User({ username: 'test', password: 'test' });
user.login(false, {
	success: function(model) {
		// redirect user to a new page
		console.debug(model);
	},
	error: function(model, response) {
		console.debug(response);
	}
});
```

You can also keep the user logged in for up to a year by flagging the first parameter "keepLoggedIn" to `true`:

```js
user.login(true, { ... })
```

On the new page (where user gets redirected), we can do various things with authentication such as:

```js
StackMob.isLoggedIn(); // this should be true since there is a logged in user

StackMob.isUserLoggedIn('test'); // this should return true since we just logged 'test' in to your webapp

StackMob.isUserLoggedIn('joshua'); // this should return false since we did not log in with user 'joshua'

var user = new StackMob.User({ username: 'test' });
user.isLoggedIn(); // this should return true since user 'test' is logged in

StackMob.getLoggedInUser(); // this should return 'test'
```

Now, let's log out our user `test`. Say your user `test` clicks on logout button.

```js
var loggedInUsername = StackMob.getLoggedInUser(); // this returns the username for the logged in user
var user = new StackMob.User({ username: loggedInUsername });
user.logout({
	success: function(model) {
		// redirect user to your log in page or some other page
		// model is undefined here
	}
});
```
