Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/geo-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Query and return all objects based on their distance from a specified GeoPoint.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* XCode 4.x and greater

* iOS SDK 5 and greater

* StackMob iOS SDK 1.3.0+

* [Download Geo Location App](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/geo-location.zip)


<h1>Let's get started!</h1>

<h2>Configure XCode Project</h2>

<h2>Open the StackMob Geo Location App</h2>

We’ll use the project from the **Saving Geo Location Data Tutorial** where we added geo location objects to our datastore.  It has StackMob imported, Core Data and a GeoPoint field defined for us.    This allows you to focus on the objective of this tutorial.

For more information on what's inside of the Geo Location app, see <a href="https://developer.stackmob.com/tutorials/ios/Saving-Geo-Location-Data" target="_blank">Saving Geo Location Data Tutorial</a>.

If you haven't already, download the Geo Location App from the link under **Prerequisites** above. 

Unzip and open **geo-location.xcodeproj**.

<h2>Add your Public Key</h2>
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** in the method:

```obj-c,4
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}
```

<h2>Add Geo Query Button</h2>

Select the storyboard file and drag and drop  a **bar button item** to your toolbar.  Set the label of the toolbar button as **Within 10 miles**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geoquery/geoquery-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)
<br/>
<br/>
To ensure you select the button inside the toolbar, open the Document Outline (you can do so by selecting Editor > Show Document Outline).  Hold the **Control** key and click on the **Bar button item - Within 10 miles** and drag to your **ViewController.h** file.  
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geoquery/geoquery-02.png">


<br>
In the menu, set the Connection to **Action** and enter the name **geoQuery**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geoquery/geoquery-03.png">


<h2>Open ViewController.h</h2>

Your ViewController.h file should look like the following:

```obj-c

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@interface ViewController : UIViewController <MKMapViewDelegate> 

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet MKMapView *mapView;

- (IBAction)saveLocation:(id)sender;
- (IBAction)geoQuery:(id)sender;

@end

```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file. We create a special SMPredicate to perform geo-queries.


```obj-c,77-110


#import "ViewController.h"
#import "AppDelegate.h"
/*
 Import the StackMob header file.
 */
#import "StackMob.h"
/*
 Import the StackMob header file.
 */
 #import "Todo.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    /*
     Set the mapView delegate.
     */
    _mapView.delegate = self;
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

- (void)viewDidUnload
{
    [self setMapView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)saveLocation:(id)sender {
     /*
     Get the current latitude and longitude in an SMGeoPoint.
     */
    [SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
        
        Todo *todo = [NSEntityDescription insertNewObjectForEntityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
        todo.todoId = [todo assignObjectId];
        todo.title = @"My Location";
        todo.location = [NSKeyedArchiver archivedDataWithRootObject:geoPoint];
        
        /*
         Save the location to StackMob.
         */
        [self.managedObjectContext saveOnSuccess:^{
            NSLog(@"Created new object in Todo schema");
        } onFailure:^(NSError *error) {
            NSLog(@"Error creating object: %@", error);
        }];
        
    } onFailure:^(NSError *error) {
        NSLog(@"Error getting SMGeoPoint: %@", error);
    }];
}

- (IBAction)geoQuery:(id)sender {
    
    /*
     Get the current location.
     */
    [SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
        
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
        NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
        [fetchRequest setEntity:entity];
        
        /*
         Set a predicate with an SMGeoPoint
         */
        SMPredicate *predicate = [SMPredicate predicateWhere:@"location" isWithin:10 milesOfGeoPoint:geoPoint];
        [fetchRequest setPredicate:predicate];
        
        [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
            NSLog(@"Successful query.");
            [results enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
                
                Todo *todo = obj;
                /*
                 To properly pull out the geo point data, unarchive the NSData into an SMGeoPoint instance.
                 */
                NSData *data = todo.location;
                SMGeoPoint *point = [NSKeyedUnarchiver unarchiveObjectWithData:data];
                NSLog(@"SMGeoPoint for object %@ in results: %@", obj, point);
            }];            
        } onFailure:^(NSError *error) {
            NSLog(@"Error: %@", error);
        }];
        
    } onFailure:^(NSError *error) {
        NSLog(@"Error getting SMGeoPoint: %@", error);
    }]; 
}
@end
```

<h2>Build and Run!</h2>

Run your project. 


If you're working with the simulator, simulate a location in the bottom pane of Xcode.

<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/geopoint/geopoint-10.png" alt=""/>
<br/>
<br/>

Allow the app to use your current location and click the **save location** button.  Now, click the **within 10 miles** button to query the results. 

Watch the debug output for a response from StackMob. 

<h3>Other types of queries</h3>
You can also perform other types of geo queries. Check out the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMPredicate.html">SMPredicate</a> class for more information.

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/geo-query.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
