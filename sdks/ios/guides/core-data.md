StackMob and Core Data
=====================================

StackMob recommends using Core Data for data persistance. It provides a powerful and robust object graph management system that otherwise would be a nightmare to implement.  Although it may have a reputation for being pretty complex, the basics are easy to grasp and understand.

# The Basics

If you are new to Core Data check out our list of <a href="http://stackmob.github.io/stackmob-ios-sdk/index.html#core_data_references" target="_blank">Core Data References</a>.

The three main pieces of Core Data are instances of:

* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectContext_Class/NSManagedObjectContext.html" target="_blank">NSManagedObjectContext</a> - This is what you use to create, read, update and delete objects in your database.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSPersistentStoreCoordinator_Class/NSPersistentStoreCoordinator.html" target="_blank">NSPersistentStoreCoordinator</a> - Coordinates between the managed object context and the actual database, in this case StackMob.
* <a href="https://developer.apple.com/library/ios/#documentation/Cocoa/Reference/CoreDataFramework/Classes/NSManagedObjectModel_Class/Reference/Reference.html" target="_blank">NSManagedObjectModel</a> - References a file where you defined your object graph.

# Coding Practices

The following are coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data.  This allows StackMob to seamlessly translate to and from the language that Core Data speaks.

## Table Mapping

This is a table of how Core Data, StackMob and regular databases map to each other:
<table cellpadding="8px" width="600px">
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

## Naming Conventions

1. **Entity Names:** Core Data entities are encouraged to start with a capital letter and will translate to all lowercase on StackMob. Example: **Superpower** entity on Core Data translates to **superpower** schema on StackMob.
2. **Property Names:** Core Data attribute and relationship names are encouraged to be in camelCase, but can also be in StackMob form, all lowercase with optional underscores. Acceptable formats are therefore **yearBorn**, **year_born**, or **yearborn**. All camelCased names will be converted to and from their equivalent form on StackMob, i.e. the property yearBorn will appear as year\_born on StackMob.
3. **StackMob Schema Primary Keys:** All StackMob schemas have a primary key field that is always schemaName\_id, unless the schema is a user object, in which case it defaults to "username" but can be changed manually by setting the userPrimaryKeyField property in your `SMClient` instance.

## Best Practices

1. **Entity Primary Keys:** Following #3 above, each Core Data entity must include an attribute of type string that maps to the primary key field on StackMob. Acceptable formats are _schemaName_Id or _schemaName_\_id. If the managed object subclass for the Entity inherits from `SMUserManagedObject`, meaning it is intended to define user objects, you may use either of the above formats or whatever lowercase string with optional underscores matches the primary key field on StackMob. For example: entity **Soda** should have attribute **sodaId** or **soda_id**, whereas your **User** entity primary key field defaults to **@"username"**. 
2. **Assign IDs:** When inserting new objects into your managed object context, you must assign an id value to the attribute which maps to the StackMob primary key field BEFORE you make save the context.
90% of the time you can get away with assigning ids like this:
		
		// assuming your instance is called newManagedObject
		[newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
		
		// now you can save your context
		
	The other 10% of the time is when you want to assign your own ids that aren't unique strings based on a UUID algorithm. A great example of this is user objects, where you would probably assign the user's name to the primary key field.  In that case, your code might look more like this:
	
		// assuming your instance is called newManagedObject
		[newManagedObject setValue:@"bob" forKey:[newManagedObject primaryKeyField]];
		
		// now you can save your context
		
3. **NSManagedObject Subclasses:** Creating an NSManagedObject subclass for each of your entities is highly recommended for convenience. You can add an init method to each subclass and include the ID assignment line from above - then you don't have to remember to do it each time you create a new object!
4. **SMUserManagedObject Subclasses:** After creating an NSManagedObject subclass for an entity that maps to a user object on StackMob, change the inherited class to SMUserManagedObject.  This class will give you a method to securely set a password for the user object, without directly setting any attributes in Core Data.  It is important to make sure you initialize an SMUserManagedObject instance properly.
5. **Create and Save Objects Before Relating Them:**  Before saving updated objects and depending on the merge policy, Core Data will grab persistent values from the server to compare against.  Problems arise when a relationship is updated with an object that hasn't been saved on the server yet.  To play it safe, try to create and save objects before relating them to one another. 
6. **Working With NSDate Attributes:** As of v1.4.0, NSDate attribute values are serialized to the StackMob server as integers in ms.  Declare the fields on StackMob as Integer.  By keeping consistency with the way the auto-generated **createddate** and **lastmoddate** fields are stored (ms), NSDate attributes for them will be deserialized correctly.

	If you want to check if one date is equal to another, use the <i>timeIntervalSinceDate:</i> method.  Dates being equal is equivalent to the time interval being < 1.  Using methods like <i>isEqualToDate:</i> track sub-second differences between dates, which are not present in the serialized integers.  Here's an example of how to check if two dates are equal:
	
		NSDate *date1 = [object1 valueForKey:@"date"];
		NSDate *date2 = [object2 valueForKey:@"date"];
		if ([date1 timeIntervalSinceDate:date2] < 1) {
			// The dates are equal.
		}

## Support Specifications

## Core Data References

* [Core Data Primer eBook](http://go.stackmob.com/learncoredata.html) - StackMob has put together an eBook based on one of our tutorial series which walks you through the full implementation of an app that uses Core Data.
* [Getting Started With Core Data](http://www.raywenderlich.com/934/core-data-on-ios-5-tutorial-getting-started) - Ray Wenderlich does a great tutorial on the basics of Core Data.
* [iPhone Core Data: Your First Steps](http://mobile.tutsplus.com/tutorials/iphone/iphone-core-data/) - Well organized tutorial on Core Data.
* [Introduction To Core Data](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/CoreData/cdProgrammingGuide.html\#//apple\_ref/doc/uid/TP40001075) - Apple's Core Data Programming Guide
* [Introduction To Predicates](https://developer.apple.com/library/ios/\#documentation/Cocoa/Conceptual/Predicates/predicates.html\#//apple\_ref/doc/uid/TP40001789) - Apple's Predicates Programming Guide

