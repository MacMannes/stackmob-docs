# Extending the StackMob JavaScript SDK

Looking to add onto the StackMob JavaScript SDK?  This guide will help you do so so that your changes are more likely to stay in the JS SDK from version to version.

# Adding new methods to StackMob

Let's add a new method to the StackMob JS SDK: `helloWorld`.  Use `_.extend(StackMob, { ... })`

```js
<script type="text/javascript">
_.extend(StackMob, {
	helloWorld: function(name) {
		console.log("hello " + name + "!");
	}
});
</script>
```

Then you could call the method:

```js
StackMob.helloWorld('bob');

//prints out "hello bob!""
```

This way, you can include any version of the StackMob JS SDK and yet still have your additions:

```js
<script type="text/javascript" src=".../stackmob-x.y.z.js"></script>
<script type="text/javascript">
_.extend(StackMob, {
	helloWorld: function(name) {
		console.log("hello " + name + "!");
	}
});
</script>
```

Now no matter what version of the StackMob JS SDK you're using, your new method will still exist.

You can use it to add constants too:

```js
_.extend(StackMob, {
	LOGIN_ERROR: "DOH!"
});

...


console.log("A login error has occurred! " + StackMob.LOGIN_ERROR);
```

# Editing existing methods

Let's assume you want to overwrite existing behavior of the JS SDK.  Let's take an example.

Let's assume you want to always fetch expanded objects (related objects) when you call `fetch`.  Here are the two methods:

**fetch**

Fetch an object.  Related objects will be returned as ID references.

```js
fetch : function(options) {
	StackMob.wrapStackMobCallbacks.call(this, options);
	Backbone.Model.prototype.fetch.call(this, options);
}
```

**fetchExpanded**

Fetch and expand an object.  Related objects will be returned as full objects.  

`fetchExpanded` is actually a regular fetch but with `options['data']['_expand']=1, 2, or 3` passed into the options.

```js
fetchExpanded : function(depth, options) {
	if(depth < 0 || depth > 3)
      StackMob.throwError('Depth must be between 0 and 3 inclusive.');

    var newOptions = { data: {
    	'_expand': depth
    }};
    _.extend(newOptions, options);


    this.fetch(newOptions);
}
```

Now, you simply want fetch to do an expanded fetch by default every time.  A trivial example, but a easy to follow one.

You can either override the method or you can edit the method to stub out the section you want.

## Overriding the method

Let's modify `fetch` so that it automatically does a fetchExpanded.  We won't edit the StackMob JS SDK.  We'll override it.

<p class="alert">Don't forget that <code>fetch</code> is a <code>StackMob.Model</code> method, so we'll be extending <code>StackMob.Model</code>.</p>

```js
<script type="text/javascript" src=".../stackmob-x.y.z.js"></script>
<script type="text/javascript">
_.extend(StackMob.Model, {
	fetch: function(options) {
	    _.extend(options, { data: {
	    	'_expand': 3
	    }});

		StackMob.wrapStackMobCallbacks.call(this, newOptions);
		Backbone.Model.prototype.fetch.call(this, newOptions);
	}
});
</script>
```

Done!  Let's use it now.

```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var todo = new Todo({ todo_id: 'abc' });
todo.fetch();  //this will now do an expanded fetch
```

## Stubbing out part of the method (Encouraged!)

Now, let's try something different - let's modify a local copy of `stackmob.js` and add a stub point.  Why, you ask?  Because by stubbing things out and submitting a GitHub pull request of the StackMob JS SDK, we can more likely pull your changes into the actual StackMob JS SDK officially!  Future developers may want to use your changes too!

**Modify StackMob.Model#fetch**

Inside `stackmob.js`, let's locate the fetch method and *modify* it.

**StackMob.Model#fetch Before**

```js
fetch : function(options) {
	StackMob.wrapStackMobCallbacks.call(this, options);
	Backbone.Model.prototype.fetch.call(this, options);
}
```

**StackMob.Model#fetch After**

Added `modifyFetchOptions`, which by default does nothing.

```js,2,7
fetch : function(options) {
	modifyFetchOptions(options);
	StackMob.wrapStackMobCallbacks.call(this, options);
	Backbone.Model.prototype.fetch.call(this, options);
},

modifyFetchOptions: function(options) { /* No op */ }, //new method that does nothing
```

Note that we added `modifyFetchOptions` but then had it do nothing.  This way the StackMob JS SDK works as is out of the box, but people now have a hook to easily override it in the future.  For example, now that we've made this change, people can now customize it themselves.

```js
<script type="text/javascript" src=".../stackmob-x.y.z.js"></script>
<script type="text/javascript">
_.extend(StackMob.Model, {
	modifyFetchOptions: function(options) {
		console.log('Adding expand to depth 1!');

		_.extend(options, { data: {
	    	'_expand': 1
	    }});
	}
});
</script>
```

We recommend the stubbing approach to give back to the community.  Assumed you've forked <a href="https://github.com/stackmob/stackmob-js-sdk">the public StackMob JavaScript SDK repository</a>, you can commit your changed to your forked repo and submit a pull request to StackMob.  We'll review it and pull your changes into the official JS SDK if things look good!

More importantly, with your changes merged in, you can upgrade the StackMob JS SDK to newer versions and the stub will be there if we've officially merged it in.

<p class="alert">Stubbing out methods to include your changes makes it much more likely that your Pull Requests will be integrated into the StackMob JS SDK.  It makes it much more likely that the JS SDK stays backwards compatible, passes tests, and hence mergable!</p>

## Overriding vs. Stubbing: What should I do?

From one developer to another, we recommend you do what you need to do to get the job done.  Both will have the same immediate effect, but investing time in stubbing out the method will help you help us future proof the StackMob JS SDK because we'll keep that method around in future versions of the SDK.  We are also grateful for community contributions and would like to see your work benefit others too!  We'd be happy to give you credit.

# Adding new methods to StackMob.Model

Let's say you want to edit the StackMob.Model.  As you know, you create new instances of objects with StackMob.Model.


```js
var Todo = StackMob.Model.extend({ schemaName: 'todo' });
var todo = new Todo();
```

Say you want to add a new method.  Extend `StackMob.Model` as you did above, but do it **after** you've run `StackMob.init`.  StackMob.init is what actually writes `StackMob.Model` and `StackMob.Collection` to the StackMob namespace.


```js
StackMob.init({
	publicKey: ...,
	apiVersion: 0
});

_.extend(StackMob.Model, {
	printID: function() {
		console.log("ID is: " + this.get(this.getPrimaryKey());
	}
});


....

todo.printID();
```

You would take a similar approach with `StackMob.Collection`.


# Overwriting the AJAX call maker

If you have your own AJAX caller (not jQuery, Zepto, or Sencha's), you can overwrite the StackMob JS SDK to use your own.  You will need a function that implements the AJAX call that takes three parameters.

* model - the `StackMob.Model` you're operating on
* params - the parameters for the AJAX request.  StackMob has prepared the headers, parameters, and everything you need for the request.
* method - the HTTP verb (GET, POST, PUT, DELETE)

Initialize the StackMob JS SDK with your function by setting it against the `ajax` field:


```js,4-6
StackMob.init({
	publicKey: ...,
	apiVersion: ...,
	ajax: function(model, params, method) {
		...
	}
});
```

The StackMob JS SDK will automatically use the `StackMob['ajax']` function for its AJAX calls if it's set.  Otherwise it will default to the jQuery, Zepto, or Sencha versions.

The `params` has all you need to make the AJAX request.  It's best to look at an example.

<a href="https://developer.stackmob.com/js-sdk/running-the-js-sdk-in-oauth1-guide" target="_blank">View an example of overwriting the StackMob JS SDK with an OAuth 1.0 AJAX caller: jsOAuth</a>

