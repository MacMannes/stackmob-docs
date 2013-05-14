Want to just see it in action? Check out the [TaskMob example app](https://github.com/stackmob/stackmob-android-examples)

<h3>Objective</h3>

Use Twitter to manage user sessions

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* An app <a href="https://developer.stackmob.com/stackmob-android-sdk/configure">set up for StackMob</a>

<h1>Let's get started!</h1>

StackMob allows you to use Twitter for user authentication in place of or allongside the built-in [User Authentication](https://developer.stackmob.com/tutorials/android/User-Authentication). This means rather than having a username and password login screen, you can have a "login with twitter" button. 

Twitter has an SDK that allows you get get back a "Twitter Token" and "Twitter Secret" for a user, which can then be used as credentials with StackMob:

```java
User newUser = new User()
newUser.createWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

Once created and thereafter, you can use the Twitter token and secret to log that user in

```java
newUser.loginWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

Alternatively, you can use the login method to create users. The method has a boolean in its parameters; when set to true, a user will be created if none exists. The user will be created with the username provided:

```java
// Passing true creates a user if needed
newUser.loginWithTwitter(twitterToken, twitterSecret, true, "[username]", new StackMobOptions(), new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});
```

If you have an existing user, created with a username/password or Facebook, you can also link it with a Twitter account to allow twitter login.

```java
existingUser.linkWithTwitter(twitterToken, twitterSecret, new StackMobModelCallback() {
	@Override
	public void success() {
	}

	@Override
	public void failure(StackMobException e) {
	}
});

```

Being logged in with Twitter is exactly the same as being logged in regularly, with extra bits of functionality. StackMobUser has two methods only usable when logged in with twitter: `postTwitterMessage` and `getTwitterUserInfo`. These allow more interation with the actual Twitter account.

Congratulations on completing this tutorial!

* Read the javadocs for [StackMobUser](http://stackmob.github.com/stackmob-java-client-sdk/javadoc/apidocs/com/stackmob/sdk/model/StackMobUser.html)
* [Browse Source Code](https://github.com/stackmob/stackmob-android-examples)