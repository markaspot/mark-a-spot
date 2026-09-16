<?php

declare(strict_types=1);

namespace Drupal\Tests\views_data_export\Kernel;

use Drupal\group\PermissionScopeInterface;
use Drupal\Tests\group\Kernel\GroupKernelTestBase;
use Drupal\views\Entity\View;
use Drupal\views\Views;
use Drupal\views_data_export\BatchProcessingAdapterInterface;
use Drupal\views_data_export\Plugin\views\display\DataExport;
use PHPUnit\Framework\Attributes\Group;

/**
 * Verifies that batch counts and rows use the same Group access checks.
 */
#[Group('views_data_export')]
final class ViewsDataExportGroupBatchTest extends GroupKernelTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = [
    'views', 'rest', 'serialization', 'csv_serialization', 'views_data_export',
  ];

  /**
   * Counts and exported IDs remain separated across accounts.
   */
  public function testGroupScopedBatchCounts(): void {
    $this->installConfig(['views']);
    $accounts = [];
    $expected = [];
    foreach (['left' => 7, 'right' => 3] as $side => $number) {
      $type = $this->createGroupType(['id' => $side]);
      $role_id = $this->createRole([], $side . '_reader');
      $account = $this->createUser([], NULL, FALSE, ['roles' => [$role_id]]);
      $accounts[$side] = $account;
      $this->createGroupRole([
        'id' => $side . '_export',
        'group_type' => $type->id(),
        'scope' => PermissionScopeInterface::OUTSIDER_ID,
        'global_role' => $role_id,
        'permissions' => ['view group'],
      ]);
      $expected[$side] = [];
      for ($i = 0; $i < $number; $i++) {
        $group = $this->createGroup([
          'type' => $side,
          'label' => $side . '-' . $i,
          'status' => 1,
        ]);
        $expected[$side][] = (int) $group->id();
      }
      // A group of the same type still needs unpublished-view permission.
      $this->createGroup(['type' => $side, 'status' => 0]);
    }

    $accounts['denied'] = $this->createUser();
    $expected['denied'] = [];

    View::create([
      'id' => 'group_batch_regression',
      'label' => 'Group batch regression',
      'base_table' => 'groups_field_data',
      'base_field' => 'id',
      'display' => [
        'default' => [
          'id' => 'default',
          'display_plugin' => 'default',
          'display_options' => [
            'access' => ['type' => 'none'],
            'cache' => ['type' => 'tag'],
            'query' => ['type' => 'views_query'],
            'fields' => [
              'id' => [
                'id' => 'id', 'table' => 'groups_field_data', 'field' => 'id',
                'plugin_id' => 'field', 'entity_type' => 'group',
                'entity_field' => 'id', 'label' => 'id',
              ],
            ],
            'sorts' => [
              'id' => [
                'id' => 'id', 'table' => 'groups_field_data', 'field' => 'id',
                'plugin_id' => 'standard', 'order' => 'ASC',
              ],
            ],
          ],
        ],
        'csv' => [
          'id' => 'csv',
          'display_plugin' => 'data_export',
          'display_options' => [
            'path' => 'group-batch-regression.csv',
            'export_method' => 'batch',
            'export_batch_size' => 2,
            'redirect_to_display' => 'none',
            'style' => ['type' => 'data_export', 'options' => ['formats' => ['csv' => 'csv']]],
          ],
        ],
      ],
    ])->save();

    // Capture batch setup without starting an HTTP batch or creating any files.
    $adapter = new class implements BatchProcessingAdapterInterface {

      /**
       * {@inheritdoc}
       */
      public function batchProcess() {
        return NULL;
      }

      /**
       * {@inheritdoc}
       */
      public function getFinishedCallback(string $caller) {
        return [$caller, 'finishBatch'];
      }

    };

    foreach (['left', 'right', 'denied', 'left'] as $side) {
      $this->setCurrentUser($accounts[$side]);
      $batch = &batch_get();
      $batch = [];
      DataExport::buildResponse('group_batch_regression', 'csv', [], $adapter);
      $operation = $batch['sets'][0]['operations'][0][1];
      $this->assertSame(count($expected[$side]), $operation[4]);
      $this->assertSame((int) $accounts[$side]->id(), (int) $operation[7]);
      $batch = [];

      // Fetch each data chunk through the regular export query path.
      $ids = [];
      for ($offset = 0; $offset < max(1, count($expected[$side])); $offset += 2) {
        $view = Views::getView('group_batch_regression');
        $view->setDisplay('csv');
        $view->setOffset($offset);
        $view->preExecute();
        $view->build();
        $view->query->setLimit(2);
        $view->execute();
        foreach ($view->result as $row) {
          $ids[] = (int) $row->_entity->id();
        }
        $view->destroy();
      }
      $this->assertSame($expected[$side], $ids);
    }
  }

}
