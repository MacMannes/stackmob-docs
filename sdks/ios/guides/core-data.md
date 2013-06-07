StackMob and Core Data
=====================================

StackMob recommends using Core Data for data persistence. It provides a powerful and robust object graph management system that otherwise would be a nightmare to implement.  Although it may have a reputation for being pretty complex, the basics are easy to grasp and understand.

## The Basics

If you are new to Core Data check out our list of <a href="#CoreDataReferences">Core Data References</a>.

The three main pieces of Core Data are instances of:

* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html" target="_blank">NSManagedObjectContext</a> - This is what you use to create, read, update and delete objects in your database.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSPersistentStoreCoordinator_Class/NSPersistentStoreCoordinator.html" target="_blank">NSPersistentStoreCoordinator</a> - Coordinates between the managed object context and the actual database, in this case StackMob.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html" target="_blank">NSManagedObjectModel</a> - References a file where you defined your object graph.

## Coding Practices

The following are coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data.  This allows StackMob to seamlessly translate to and from the language that Core Data speaks.

### Table Mapping

This is a table of how Core Data, StackMob and regular databases map to each other:
<table class="table table-bordered table-pretty-header">
	<tr align="center">
		<th>Core Data</th>
		<th>StackMob</th>
		<th>Database</th>
	</tr>
	<tr>
		<td>Entity</td>
		<td>Schema</td>
		<td>Table</td>
	</tr>
	<tr>
		<td>Attribute</td>
		<td>Field</td>
		<td>Column</td>
	</tr>
	<tr>
		<td>Relationship</td>
		<td>Relationship</td>
		<td>Reference Column</td>
	</tr>
</table>

### Naming Conventions

#### Entity Names

Core Data entities are encouraged to start with a capital letter and will translate to all lowercase on StackMob. Example: **Superpower** entity on Core Data translates to **superpower** schema on StackMob.

#### Property Names

Core Data attribute and relationship names are encouraged to be in camelCase, but can also be in StackMob form, all lowercase with optional underscores. Acceptable formats are therefore **yearBorn**, **year_born**, or **yearborn**. All camelCased names will be converted to and from their equivalent form on StackMob, i.e. the property yearBorn will appear as year\_born on StackMob.

#### StackMob Schema Primary Keys

All StackMob schemas have a primary key field that is always **schemaName\_id**, unless the schema is a user object, in which case it defaults to "username" but can be changed manually by setting the `userPrimaryKeyField` property in your `SMClient` instance.

### Best Practices

#### Entity Primary Keys

Following the section above on primary keys, each Core Data entity must include an attribute of type string that maps to the primary key field on StackMob. Acceptable formats are <b><i>schemaName</i>Id</b> or <b><i>schemaName</i>_id</b>. 

If the managed object subclass for the Entity inherits from `SMUserManagedObject`, meaning it is intended to define user objects, you may use either of the above formats or whatever lowercase string with optional underscores matches the primary key field on StackMob. 

For example: entity **Soda** should have attribute **sodaId** or **soda_id**, whereas your **User** entity primary key field defaults to **@"username"**.

#### Assign IDs

When inserting new objects into your managed object context, you must assign an id value to the attribute which maps to the StackMob primary key field BEFORE you make save the context. 90% of the time you can get away with assigning ids like this:


```obj-c
// assuming your instance is called newManagedObject
[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];

// now you can save your context
```


The other 10% of the time is when you want to assign your own ids that aren't unique strings based on a UUID algorithm. A great example of this is user objects, where you would probably assign the user's name to the primary key field.  In that case, your code might look more like this:


```obj-c
// assuming your instance is called newManagedObject
[newManagedObject setValue:@"bob" forKey:[newManagedObject primaryKeyField]];

// now you can save your context
```


#### NSManagedObject Subclasses

Creating an NSManagedObject subclass for each of your entities is highly recommended for convenience. You can add an init method to each subclass and include the ID assignment line from above - then you don't have to remember to do it each time you create a new object!

#### SMUserManagedObject Subclasses

After creating an NSManagedObject subclass for an entity that maps to a user object on StackMob, change the inherited class to SMUserManagedObject.  This class will give you a method to securely set a password for the user object, without directly setting any attributes in Core Data.  It is important to make sure you initialize an SMUserManagedObject instance properly.

#### Create Before Relate

Before saving updated objects and depending on the merge policy, Core Data will grab persistent values from the server to compare against.  Problems arise when a relationship is updated with an object that hasn't been saved on the server yet.  To play it safe, try to create and save objects before relating them to one another. 

#### Working With NSDate Attributes

As of v1.4.0, NSDate attribute values are serialized to the StackMob server as integers in ms.  Declare the fields on StackMob as Integer.  By keeping consistency with the way the auto-generated **createddate** and **lastmoddate** fields are stored (ms), NSDate attributes will be deserialized correctly.

If you want to check if one date is equal to another, use the **timeIntervalSinceDate:** method.  Dates being equal to the second is equivalent to the time interval being less than 1.  Methods like <b>isEqualToDate:</b> track sub-second differences between dates, which are not present in the serialized integers.  Here's an example of how to check if two dates are equal:


```obj-c
NSDate *date1 = [object1 valueForKey:@"date"];
NSDate *date2 = [object2 valueForKey:@"date"];
if ([date1 timeIntervalSinceDate:date2] < 1) {
	// The dates are equal.
}
```

Feel free to try other ways of comparing dates to find the method that works best for your app's needs.

## Support Specifications

As part of the integration between StackMob and Core Data, our goal is to support as much Core Data functionality as we can, including attribute types, fetch request settings and predicate formats.

As we continually improve our SDK this page will stay up to date with everything that is currently supported.

Features without date to indicate starting support date are assumed v1.0.0+

### Data Type Maps

##### Core Data Attribute Type to StackMob Field Type

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>Core Data Attribute Type</th>
        <th>StackMob Field Type</th>
    </tr>
    <tr>
        <td>Integer 16</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Integer 32</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Integer 64</td>
        <td>Integer</td>
    </tr>
    <tr>
        <td>Decimal</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>Double</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>Float</td>
        <td>Float</td>
    </tr>
    <tr>
        <td>String</td>
        <td>String, Binary (see <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMBinaryDataConversion.html">SMBinaryDataConversion</a>)</td>
    </tr>
    <tr>
        <td>Boolean <i>(v1.1.1+)</i></td>
        <td>Boolean</td>
    </tr>
    <tr>
        <td>NSDate</td>
        <td>Integer, Representing the number of milliseconds since 1970</td>
    </tr>
    <tr>
        <td>Binary Data</td>
        <td>Currently Not Supported</td>
    </tr>
    <tr>
        <td>Transformable <i>(v1.3.0+)</i></td>
        <td>Geopoint (lat/lon) - See <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data">Saving Geo Location Data Tutorial</td>
    </tr>
</table>

##### StackMob Field Type to Core Data Attribute Type

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>StackMob Field Type</th>
        <th>Core Data Attribute Type</th>
    </tr>
    <tr>
        <td>String</td>
        <td>String</td>
    </tr>
    <tr>
        <td>Integer</td>
        <td>Integer 16, 32, & 64</td>
    </tr>
    <tr>
        <td>Float</td>
        <td>Decimal</td>
    </tr>
    <tr>
        <td>Boolean</td>
        <td>Boolean</td>
    </tr>
    <tr>
        <td>Array</td>
        <td>Currently not supported with Core Data, use the Datastore API (See <a href="https://developer.stackmob.com/tutorials/ios/Lower-Level-CRUD-API">Lower Level CRUD API Tutorial</a>)</td>
    </tr>
    <tr>
        <td>Binary</td>
        <td>String</td>
    </tr>
    <tr>
        <td>Geopoint (lat/lon) <i>(v1.3.0+)</i></td>
        <td>Transformable - See <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data">Saving Geo Location Data Tutorial</a></td>
    </tr>
</table>

### Fetch Requests

Support Table is based on `NSFetchRequest` methods.  Additional support, especially in regards to managing how results are returned, is planned for future releases.

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>NSFetchRequest method</th>
        <th>Supported</th>
    </tr>
    <tr>
        <td>fetchRequestWithEntityName:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>entityName / initWithEntityName:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>entity / setEntity:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>includesSubentities / setIncludesSubEntities:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>predicate / setPredicate:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchLimit / setFetchLimit:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchOffset / setFetchOffset:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>fetchBatchSize / setFetchBatchSize:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>affectedStores / setAffectedStores:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>sortDescriptors / setSortDescriptors:</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>relationshipKeyPathsForPrefetching / setRelationshipKeyPathsForPrefetching:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>resultType / setResultType:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>includesPendingChanges / setIncludesPendingChanges:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>propertiesToFetch / setPropertiesToFetch:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>returnsDistinctRestults / setReturnsDistinctResults:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>includesPropertyValues / setIncludesPropertyValues:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>shouldRefreshRefetchedObjects / setShouldRefreshRefetchedObjects:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>returnObjectsAsFaults / setReturnObjectsAsFaults:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>propertiesToGroupBy / setPropertiesToGroupBy:</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>havingPredicate / setHavingPredicate:</td>
        <td>NO</td>
    </tr>
</table>

<b>Additional Supported Features:</b>
<ul>
	<li>Fetch Entity with predicates where "relationship key == managed object or managed object ID". <b>Important:</b> Predicate supported only for comparison predicates and to-one relationships.</li>
</ul>
<b>Not Supported</b>
<ul>
	<li>Fetching Entity with key paths in predicate, i.e. "relationshipKey.attribute == value".</li>
	<li>Fetching Geo-point data from the cache.</li>
</ul>


#### Predicates

##### <b>Comparison Predicates</b>

<br/>

Predicate's <b>leftExpression</b> must be of type <b>NSKeyPathExpressionType</b>.

Predicate's <b>rightExpression</b> must be of type <b>NSConstantValueExpressionType</b>.

<br/>

##### Supported Predicate Operator Types (based on NSPredicateOperatorType):

<br/>

<table class="table table-bordered table-pretty-header">
    <tr align="center">
        <th>NSPredicateOperatorType</th>
        <th>Supported</th>
    </tr>
    <tr>
        <td>NSLessThanPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSLessThanOrEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSGreaterThanPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSGreaterThanOrEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSNotEqualToPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSMatchesPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSLikePredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSBeginsWithPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSEndsWithPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSInPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
    <tr>
        <td>NSCustomSelectorPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSContainsPredicateOperatorType</td>
        <td>NO</td>
    </tr>
    <tr>
        <td>NSBetweenPredicateOperatorType</td>
        <td><b>YES</b></td>
    </tr>
</table>

##### <b>Compound Predicates</b>

<p>For a tutorial on using compound predicates see the <a href="https://developer.stackmob.com/tutorials/ios/Advanced-Queries" target="_blank">Advanced Query Tutorial</a>.
<ul>
	<li>NSAndPredicateType - Use [NSPredicate andPredicateWithSubpredicates:arrayOfPredicates] or AND in predicate format string.</li>
	<li>NSORPredicateType - Use [NSPredicate orPredicateWithSubpredicates:arrayOfPredicates] or OR in predicate format string.</li>
</ul>

### Concurrency API

The concurrency API <i>(v1.2.0+)</i> brings a new set of asynchronous and synchronous operations to Core Data.  All methods follow patterns to execute saves and fetches on background queues on private contexts.  It is recommended to use these methods to achieve optimal performance between Core Data, StackMob and the local caching system.

All methods can be found in the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">SMManagedObjectContext+Concurrency Class Reference</a>.

### Miscellaneous Support

#### Default Values

Setting default values for attributes of string, number (integer, decimal, etc.), or boolean types will get included in the object dictionary during insert.

<b>Check your attributes with number types</b> because they automatically default to 0 unless you deselect the `Default` checkbox.

## Core Data References

* [Core Data Primer eBook](http://go.stackmob.com/learncoredata.html) - StackMob has put together an eBook based on one of our tutorial series which walks you through the full implementation of an app that uses Core Data.
* [Getting Started With Core Data](http://www.raywenderlich.com/934/core-data-on-ios-5-tutorial-getting-started) - Ray Wenderlich does a great tutorial on the basics of Core Data.
* [iPhone Core Data: Your First Steps](http://mobile.tutsplus.com/tutorials/iphone/iphone-core-data/) - Well organized tutorial on Core Data.
* [Introduction To Core Data](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/CoreData/cdProgrammingGuide.html\#//apple\_ref/doc/uid/TP40001075) - Apple's Core Data Programming Guide
* [Introduction To Predicates](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/Predicates/predicates.html\#//apple\_ref/doc/uid/TP40001789) - Apple's Predicates Programming Guide
