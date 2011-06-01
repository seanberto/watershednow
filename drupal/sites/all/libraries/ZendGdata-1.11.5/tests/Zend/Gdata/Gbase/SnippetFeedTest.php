<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_Gdata_Gbase
 * @subpackage UnitTests
 * @copyright  Copyright (c) 2005-2011 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id $
 */

require_once 'Zend/Gdata/Gbase.php';
require_once 'Zend/Http/Client.php';

/**
 * @category   Zend
 * @package    Zend_Gdata_Gbase
 * @subpackage UnitTests
 * @copyright  Copyright (c) 2005-2011 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @group      Zend_Gdata
 * @group      Zend_Gdata_Gbase
 */
class Zend_Gdata_Gbase_SnippetFeedTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
        $this->snippetFeed = new Zend_Gdata_Gbase_SnippetFeed(
                file_get_contents(dirname(__FILE__) . '/_files/TestDataGbaseSnippetFeedSample1.xml'),
                true);
    }

    public function testToAndFromString()
    {
        $this->assertEquals(2, count($this->snippetFeed->entries));
        $this->assertEquals(2, $this->snippetFeed->entries->count());
        foreach($this->snippetFeed->entries as $entry)
        {
            $this->assertTrue($entry instanceof Zend_Gdata_Gbase_SnippetEntry);
        }

        $newSnippetFeed = new Zend_Gdata_Gbase_SnippetFeed();
        $doc = new DOMDocument();
        $doc->loadXML($this->snippetFeed->saveXML());
        $newSnippetFeed->transferFromDom($doc->documentElement);

        $this->assertEquals(2, count($newSnippetFeed->entries));
        $this->assertEquals(2, $newSnippetFeed->entries->count());
        foreach($newSnippetFeed->entries as $entry)
        {
            $this->assertTrue($entry instanceof Zend_Gdata_Gbase_SnippetEntry);
        }
    }

}
